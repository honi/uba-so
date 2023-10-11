#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "letras123"

typedef struct {
	char letter;
} private_data;

int available = 3;
struct semaphore mutex;

int mod_open(struct inode *inode, struct file *file) {
	if (down_interruptible(&mutex)) return -ERESTARTSYS;

	if (available == 0) {
		up(&mutex);
		return -EPERM;
	} else {
		available--;
		up(&mutex);
		file->private_data = kmalloc(sizeof(private_data), GFP_KERNEL);
		private_data* pd = (private_data*) file->private_data;
		pd->letter = 0;
	}

	return 0;
}

int mod_release(struct inode *inode, struct file *file) {
	if (file->private_data == NULL) return -EPERM;
	if (down_interruptible(&mutex)) return -ERESTARTSYS;

	available++;
	up(&mutex);
	kfree(file->private_data);

	return 0;
}

ssize_t mod_read(struct file *file, char __user *ubuffer, size_t count, loff_t *pos) {
	int i = 0;
	if (file->private_data == NULL) return -EPERM;

	private_data* pd = (private_data*) file->private_data;
	if (pd->letter == 0) return -EPERM;

	char *kbuffer = (char*)kmalloc(count, GFP_KERNEL);
	for (i = 0; i < count; i++) kbuffer[i] = pd->letter;
	copy_to_user(ubuffer, kbuffer, count);
	kfree(kbuffer);

	return count;
}

ssize_t mod_write(struct file *file, const char __user *ubuffer, size_t count, loff_t *pos) {
	if (file->private_data == NULL) return -EPERM;

	private_data* pd = (private_data*) file->private_data;
	if (pd->letter == 0) copy_from_user(&pd->letter, ubuffer, 1);

	return count;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = mod_open,
	.release = mod_release,
	.read = mod_read,
	.write = mod_write,
};
static struct cdev dev = {};
static struct class *dev_class;
dev_t dev_num;

static int __init mod_init(void) {
	cdev_init(&dev, &fops);
	alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	cdev_add(&dev, dev_num, 1);

	dev_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(dev_class, NULL, dev_num, NULL, DEVICE_NAME);

	sema_init(&mutex, 1);

	return 0;
}

static void __exit mod_exit(void) {
	unregister_chrdev_region(dev_num, 1);
	cdev_del(&dev);

	device_destroy(dev_class, dev_num);
	class_destroy(dev_class);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DEVICE_NAME);
MODULE_AUTHOR("JB");

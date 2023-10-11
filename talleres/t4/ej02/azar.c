#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "azar"

int max_number = -1;

int get_digits(int number) {
	int digits = 0;
	while (number > 0) {
		number /= 10;
		digits++;
	}
	return digits;
}

ssize_t mod_read(struct file *file, char __user *ubuffer, size_t count, loff_t *pos) {
	unsigned int number;
	int size;
	char* kbuffer;

	if (max_number == -1) return -EPERM;

	get_random_bytes(&number, sizeof(number));
	number = number % max_number;

	size = get_digits(number) + 1;
	kbuffer = (char*)kmalloc(size, GFP_KERNEL);
	snprintf(kbuffer, size + 1, "%d\n", number);
	copy_to_user(ubuffer, kbuffer, size);
	kfree(kbuffer);

	printk(KERN_INFO "mod_read: %u / %d\n", number, size);

	return size;
}

ssize_t mod_write(struct file *file, const char __user *ubuffer, size_t count, loff_t *pos) {
	char *kbuffer = (char*)kmalloc(count + 1, GFP_KERNEL);
	copy_from_user(kbuffer, ubuffer, count);
	kbuffer[count] = '\0';
	kstrtoint(kbuffer, 10, &max_number);
	kfree(kbuffer);
	return count;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
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

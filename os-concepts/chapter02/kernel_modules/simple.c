#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>
#include <asm/param.h>

long unsigned int start_jiffies;

/* This function is called when the module is loaded. */
static int __init simple_init(void) {
	printk(KERN_INFO "Loading Kernel Module\n");
	printk(KERN_INFO "GOLDEN_RATIO_PRIME = %llu\n", GOLDEN_RATIO_PRIME);
	printk(KERN_INFO "jiffies = %lu\n", jiffies);
	printk(KERN_INFO "HZ = %d\n", HZ);
	start_jiffies = jiffies;
	return 0;
}

/* This function is called when the module is removed. */
static void __exit simple_exit(void) {
	printk(KERN_INFO "Removing Kernel Module\n");
	printk(KERN_INFO "gcd(3300, 24) = %lu\n", gcd(3300, 24));
	printk(KERN_INFO "jiffies = %lu\n", jiffies);
	printk(KERN_INFO "Kernel Module was loaded for %lums\n", 1000 / HZ * (jiffies - start_jiffies));
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("JB");


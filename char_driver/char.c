// Author: Yashwanth

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>

#define DEVICE_NAME "yash"
#define	CLASS_NAME "ykl"

MODULE_AUTHOR("Yashwanth");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple character device");
MODULE_VERSION("1.0");

static int majornum;
static struct class *classobj = NULL;
static struct device *devobj = NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops = 
{
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
};

static int __init device_init(void)
{
	printk(KERN_INFO "yash: Initializing the device yash\n");

	majornum = register_chrdev(0, DEVICE_NAME, &fops);
	if(majornum < 0)
	{
		printk(KERN_INFO "yash: invalid major number\n");
		return majornum;
	}

	classobj = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(classobj))
	{
		unregister_chrdev(majornum, DEVICE_NAME);
		printk(KERN_INFO "yash: failed to create device class\n");
		return PTR_ERR(classobj);
	}

	devobj = device_create(classobj, NULL, MKDEV(majornum, 0), NULL, DEVICE_NAME);
	if(IS_ERR(devobj))
	{
		class_destroy(classobj);
		unregister_chrdev(majornum, DEVICE_NAME);
		printk(KERN_INFO "yash: failed to create device reference\n");
		return PTR_ERR(devobj);
	}
	printk(KERN_INFO "yash: device /dev/yash created and initialized successfully\n");
}

static void __exit device_exit(void)
{
	device_destroy(classobj, MKDEV(majornum, 0));
	class_unregister(classobj);
	class_destroy(classobj);
	unregister_chrdev(majornum, DEVICE_NAME);
	printk("yash: Exiting now..\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "yash: Opening device /dev/yash\n");
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "yash: Releasing the file struct\n");
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buf, size_t len, loff_t *offset)
{
	printk(KERN_INFO "yash: Reading from device /dev/yash\n");
	return 0;
}

static ssize_t dev_write(struct file *filep, const char *buf, size_t len, loff_t *offset)
{
	printk(KERN_INFO "yash: Writing to device /dev/yash\n");
	return 0;
}

module_init(device_init);
module_exit(device_exit);


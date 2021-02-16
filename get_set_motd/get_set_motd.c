#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "cust_motd"
#define CLASS_NAME "motd"


MODULE_AUTHOR("Yashwanth");
MODULE_DESCRIPTION("Get and set message of the day");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static char *motd = "Hello userland!";

static struct file_operations fops = {

	.read = dev_read,
	.write = dev_write,
	.open = dev_open,
	.release = dev_release,
};

static int major;
static struct class *classobj = NULL;
static struct device *deviceobj = NULL;
static int open_count = 0;

static int __init mod_init(void)
{

	printk(KERN_INFO "Initilizing the device driver\n");

	major = register_chrdev(0, DEVICE_NAME, &fops);

	if(major < 0)
	{
		printk(KERN_INFO "Failed to allocate major number\n");
		return major;
	}

	classobj = class_create(THIS_MODULE, CLASS_NAME);
	if(classobj == NULL)
	{
		printk(KERN_INFO "Failed to create device class\n");
		unregister_chrdev(major, DEVICE_NAME);
		return PTR_ERR(classobj);

	}

	deviceobj = device_create(classobj, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

	if(deviceobj==NULL)
	{
		printk(KERN_INFO "Failed to create device\n");
		class_destroy(classobj);
		unregister_chrdev(major, DEVICE_NAME);
		return PTR_ERR(deviceobj);
	}
	
	printk(KERN_INFO "Device cust_motd created under /dev successfully \n");
	return 0;	
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "Removing the module get_set_motd\n");
	device_destroy(classobj, MKDEV(major, 0));
	class_unregister(classobj);
	class_destroy(classobj);
	unregister_chrdev(major, DEVICE_NAME);
	printk(KERN_INFO "Exiting..\n");
}

static int dev_open(struct inode *inode, struct file *file)
{
	open_count++;

	if(open_count > 1)
	{
		printk(KERN_INFO "Device already in use. Try again later..\n");
		open_count--;
		return -EBUSY;
	}

	printk(KERN_INFO "Opening device /dev/cust_motd\n");
	return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
	if(open_count > 0)
	{
		printk(KERN_INFO "Releasing file struct\n");
		open_count--;
		return 0;
	}
	else 
		return 1;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	uint32_t bytes_read = 0;
	static char *temp;

	// Using temp variable to preserve the original MOTD
	temp = motd;

	printk(KERN_INFO "Reading from /dev/cust_motd\n");
	while(*temp && len)
	{
		put_user(*(temp++), buf++);
		len--;
		bytes_read++;
//		printk(KERN_INFO "MOTD: %s", motd);
	}
	return bytes_read;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{

	printk(KERN_INFO "Writing to /dev/cust_motd\n");

	//limit MOTD to 50 bytes
	static char copy_buf[50];
	uint32_t bytes_written = 0, i = 0;

	if(len <= 50)
	{
		while(len)
		{
			get_user(copy_buf[i], buf++);
			len--;
			i++;
			bytes_written++;
		}
	}

	motd = copy_buf;
	printk(KERN_INFO "MOTD changed successfully to %s\n", motd);

	return bytes_written;

	return 0;
}


module_init(mod_init);
module_exit(mod_exit);

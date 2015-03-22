#include <linux/module.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

struct hello_dev
{
	// 此处可以放很多私有变量
	struct cdev mydev;
};
struct hello_dev* hello_devices;

char temp[128]={0};

ssize_t hello_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{ 
	printk(KERN_ERR "icecut: hello_read\n");
    if (copy_to_user(buffer, temp, count))  
            return -EFAULT;  
    return count; 
}

ssize_t hello_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
	printk(KERN_ERR "icecut: hello_write\n");
	if (copy_from_user(temp, buffer, count))
	{
		return -EFAULT;
	}
	return count;
}

int hello_open(struct inode *inode, struct file *file)
{
	printk(KERN_ERR "icecut: hello_open\n");
	return 0;
}

int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_ERR "icecut: hello_release\n");
	return 0;
}

struct file_operations hello_fops = {  
    .owner =    THIS_MODULE,  
    .read =     hello_read,  
    .write =    hello_write,  
    .open =     hello_open,  
    .release =  hello_release,  
}; 

static int __init hello_init(void)
{
	int ret = 0;
	dev_t dev = MKDEV(250, 0);
	/*驱动加载时会来此执行初始化代码*/
	printk(KERN_ERR "icecut: hello_init\n");
	// 申请设备号
	ret = register_chrdev_region(dev, 1, "hello");
	if (ret < 0)
	{
		printk(KERN_ERR "alloc major error\n");
		return ret;
	}
	printk(KERN_ERR "alloc major %d-monor %d\n", MAJOR(dev), MINOR(dev));
	// 创建当前驱动需要的内存区
	hello_devices = (struct hello_dev*)kmalloc(sizeof(struct hello_dev), GFP_KERNEL);
	if (!hello_devices)
	{
		printk(KERN_ERR "alloc memory error\n");
		ret = -ENOMEM;
		goto fail;
	}
	memset(hello_devices, 0, sizeof(struct hello_dev));

	// 初始化设备
	cdev_init(&hello_devices->mydev, &hello_fops);
	hello_devices->mydev.owner = THIS_MODULE;
	hello_devices->mydev.ops = &hello_fops;
	ret = cdev_add(&hello_devices->mydev, dev, 1);// 添加字符设备
	if (ret)
	{
		printk(KERN_ERR "cdev_add error %d\n", ret);
		goto fail1;
	}
	printk(KERN_ERR "init success %d\n", ret);
	return 0;
fail1:
	kfree(hello_devices);
fail:
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	/*驱动卸载时会来此执行清理代码*/
	printk(KERN_ERR "icecut: hello_exit\n");
}
module_exit(hello_exit);

MODULE_LICENSE("GPL");

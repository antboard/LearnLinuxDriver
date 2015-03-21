#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void)
{
	/*驱动加载时会来此执行初始化代码*/
	printk(KERN_ERR "icecut: hello_init\n");
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

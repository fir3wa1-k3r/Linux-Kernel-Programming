#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>


MODULE_AUTHOR("Yashwanth");
MODULE_DESCRIPTION("Simple module that implements kthreads");
MODULE_LICENSE("GPL");

struct task_struct *thread1, *thread2;

static int func_kthrd(void *arg) 
{
	int i;
	for (i=0; i<5; i++)
		pr_info("Hello[%d] from thread: %d\n", i, current->pid);

	return 0;
}

static int __init init_mod(void)
{

	pr_info("Module loaded. Creating 2 threads...\n");

	thread1 = kthread_create(&func_kthrd, NULL, "thread_1");
	if(IS_ERR(thread1))
	{
		pr_info("Unable to create thread 1\n");
		thread1 = NULL;
		return 1;
	}

	thread2 = kthread_create(&func_kthrd, NULL, "thread_2");
	if(IS_ERR(thread2))
	{
		pr_info("Unable to create thread 2\n");
		thread2 = NULL;
		return 1;
	}

	wake_up_process(thread1);
	wake_up_process(thread2);

	return 0;
}

static void __exit exit_mod(void)
{
	pr_info("Unloading module\n");
}


module_init(init_mod);
module_exit(exit_mod);

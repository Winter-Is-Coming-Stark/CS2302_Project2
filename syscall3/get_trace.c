#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
MODULE_LICENSE("Dual BSD/GPL");
#define __NR_get_trace 363

static int (*oldcall)(void);

static int sys_get_trace(pid_t pid, int __user *wcounts)
{
		struct task_struct *tsk = NULL;
		struct pid * PID;
		PID = find_vpid(pid);
		if (!PID) return -EINVAL;
		tsk = get_pid_task(PID, PIDTYPE_PID);
		int k_wcounts = 0;
		//tsk = dfs(&init_task, pid);
		//printk(KERN_INFO "%d",init_task.pid);
		if(!tsk){
			printk(KERN_INFO "tsk is NULL!\n");
			return -EINVAL;
		}
		else {
				if(!tsk->trace_flag){
						printk(KERN_INFO "kernel is not tracing!\n");
						return -EINVAL;
				}
				k_wcounts = tsk->wcounts;
				//printk(KERN_INFO "kernel count = %d", tsk->wcounts);
				if(put_user(k_wcounts, wcounts) == -EFAULT){
						printk(KERN_INFO "Error copying to user!\n");
						return -EFAULT;
				}
		}
		return 0;
}



static int addsyscall_init(void)
{
		long *syscall = (long*)0xc000d8c4;
		oldcall = (void(*)(void))(syscall[__NR_get_trace]);
		syscall[__NR_get_trace] = (unsigned long)sys_get_trace;
		printk(KERN_INFO "module get trace load!\n");
		return 0;
}

static void addsyscall_exit(void)
{
		long *syscall = (long*)0xc000d8c4;
		syscall[__NR_get_trace] = (unsigned long)oldcall;
		printk(KERN_INFO "module get trace exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
MODULE_LICENSE("Dual BSD/GPL");
#define __NR_stop_trace 362

static int (*oldcall)(void);

static int sys_stop_trace(pid_t pid)
{
		struct task_struct *tsk = NULL;
		struct pid * PID;
		PID = find_vpid(pid);
		if (!PID) return -EINVAL;
		tsk = get_pid_task(PID, PIDTYPE_PID);
		//tsk = dfs(&init_task, pid);
		//printk(KERN_INFO "%d",init_task.pid);
		if(!tsk){
			printk(KERN_INFO "tsk is NULL!\n");
			return -EINVAL;
		}
		else {
			printk(KERN_INFO "%d\n", tsk->trace_flag);
			tsk->trace_flag = 0;
			//printk(KERN_INFO "%d %d\n", tsk->trace_flag, tsk->wcounts);
			//printk(KERN_INFO "%d\n", tsk->pid);
		}
		return 0;
}



static int addsyscall_init(void)
{
		long *syscall = (long*)0xc000d8c4;
		oldcall = (void(*)(void))(syscall[__NR_stop_trace]);
		syscall[__NR_stop_trace] = (unsigned long)sys_stop_trace;
		printk(KERN_INFO "module stop trace load!\n");
		return 0;
}

static void addsyscall_exit(void)
{
		long *syscall = (long*)0xc000d8c4;
		syscall[__NR_stop_trace] = (unsigned long)oldcall;
		printk(KERN_INFO "module stop trace exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
MODULE_LICENSE("Dual BSD/GPL");
#define __NR_start_trace 361

static int (*oldcall)(void);
/*
static struct task_struct *dfs(struct task_struct* cur_task, pid_t pid)
{
		if(!cur_task){
				printk(KERN_INFO "cur_task is NULL!\n");
				return NULL;
		}
		struct list_head *lh_child, *lh_children;
		struct task_struct *child_task, *rt;
		printk(KERN_INFO "%d", cur_task->pid);
		if(cur_task->pid == pid) return cur_task;
		lh_children = &(cur_task->children);
		list_for_each(lh_child, lh_children){
				child_task = list_entry(lh_child, struct task_struct, sibling);
				rt = dfs(child_task, pid);
				if(rt != NULL) return rt;
		}
		return NULL;
}
*/

static int sys_start_trace(pid_t pid)
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
			if(tsk->trace_flag == 1){
					printk(KERN_INFO "cannot call start trace twice!\n");
					return -EINVAL;
			}
			tsk->trace_flag = 1;
			tsk->wcounts = 0;
			printk(KERN_INFO "%d %d\n", tsk->trace_flag, tsk->wcounts);
			printk(KERN_INFO "%d\n", tsk->pid);
			return 0;
		}
}



static int addsyscall_init(void)
{
		long *syscall = (long*)0xc000d8c4;
		oldcall = (void(*)(void))(syscall[__NR_start_trace]);
		syscall[__NR_start_trace] = (unsigned long)sys_start_trace;
		printk(KERN_INFO "module start trace load!\n");
		return 0;
}

static void addsyscall_exit(void)
{
		long *syscall = (long*)0xc000d8c4;
		syscall[__NR_start_trace] = (unsigned long)oldcall;
		printk(KERN_INFO "module start trace exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

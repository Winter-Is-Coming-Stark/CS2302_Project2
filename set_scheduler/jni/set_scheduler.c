#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <linux/sched.h>
#include <errno.h>


#define SCHED_NORMAL		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_BATCH		3
/* SCHED_ISO: reserved but not implemented yet */
#define SCHED_IDLE		5
#define SCHED_RAS       6

char *policy_name[7] = {
    "SCHED_NORMAL",
    "SCHED_FIFO",
    "SCHED_RR",
    "SCHED_BATCH",
    "ERROR",
    "SCHED_IDLE",
    "SCHED_RAS"
};

int main(){
    int sched_policy, pid, wcount, prio, prev_policy;
    struct sched_param params;
    printf("Please input the Choice of Scheduling algorithms (0 - NORMAL, 1 - FIFO, 2 - RR, 6 - RAS) : \n");
    scanf("%d", &sched_policy);
    printf("Current sched algorithm is %s\n", policy_name[sched_policy]);
    printf("Please input the id (PID) of the testprocess : \n");
    scanf("%d", &pid);
	syscall(361, pid);
    syscall(363, pid, &wcount);
	syscall(362, pid);
    printf("Wcount for this process is : %d\n", wcount);
    printf("set process's priority(1-99) : \n");
    scanf("%d", &prio);
	prio = 9 - prio;
	params.sched_priority = prio;
    prev_policy = sched_getscheduler(pid);
    if(prev_policy < 0 || prev_policy > 6 || prev_policy == 5){
        printf("Failed to get scheduler!\n");
        return 0;
    }
    if(sched_setscheduler(pid, sched_policy, &params)){
        printf("Failed to set sheduler!\n");
		return 0;
    }
    
    sched_policy = sched_getscheduler(pid);
    if(sched_policy < 0 || sched_policy > 6 || sched_policy == 5){
        printf("Failed to get scheduler!\n");
        return 0; 
    }
    printf("Pre scheduler : %s\n", policy_name[prev_policy]);
    printf("cur scheduler : %s\n", policy_name[sched_policy]);
    return 0;
}

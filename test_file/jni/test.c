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
#include <pthread.h>
#include <sys/syscall.h>

void *func();

int main(){
		int i, j;
		pthread_t thread1, thread2;
		/*
		if(pid == 0){
			if(sched_setscheduler(getpid(), 6, &params)){
					printf("Failed to set sheduler!\n");
					return 0;
			}
		 }
		else {
			if(sched_setscheduler(getpid(), 6, &params)){
					printf("Failed to set sheduler!\n");
					return 0;
			}
		 }
		
		if(pid){
				int status = 0;
				if(waitpid(pid, &status, 0) < 0){
						printf("wait failed!\n");
						return 0;
				}
		}
		*/
		int iret1, iret2, ret;
		pthread_attr_t tattr;
		int policy;

		/* set the scheduling policy to SCHED_OTHER */
		/*
		ret = pthread_attr_setschedpolicy(&tattr, 6);
		if(ret){
				printf("error");
				return 0;
		}
		*/
		/*
		iret1 = pthread_create(&thread1, NULL, func, NULL);
		if(iret1){
				fprintf(stderr, "Error - pthread_create() return code %d\n", iret1);
		}
		iret2 = pthread_create(&thread2, NULL, func, NULL);
		if(iret2){
				fprintf(stderr, "Error - pthread_create() return code %d\n", iret2);
		}
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		*/

		struct sched_param params;
		params.sched_priority = 0;
		if(sched_setscheduler(getpid(), 6, &params)){
		printf("Failed to set sheduler!\n");
			return 0;
		}
		while(1);
		return 0;
}

void *func(){
		int i;
		pid_t tid;
		struct sched_param params;
		tid = gettid();
		params.sched_priority = 0;
			if(sched_setscheduler(tid, 6, &params)){
					printf("Failed to set sheduler!\n");
					return;
			}
		for(i = 0;i < 100000000;i++){
				if(i % 10000000 == 0) printf("%d %d \n", getpid(), i / 10000000);
		}
}

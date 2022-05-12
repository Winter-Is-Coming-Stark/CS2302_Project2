#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <linux/sched.h>

static int alloc_size;
static char* memory;
void segv_handler(int sig_number){
	//printf("find memory accessed!\n");
    mprotect(memory, alloc_size, PROT_READ | PROT_WRITE);
}
pid_t pid;
int proc_id = 0;
int number = 0;

int main(){
    printf("please input the number of processes : \n");
    scanf("%d", &number);
	number -= 1;

    int fd, wcounts = 0, j;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &segv_handler;
    sigaction(SIGSEGV, &sa, NULL);

    alloc_size = getpagesize();
    fd = open("/dev/zero", O_RDONLY);
    memory = mmap(NULL, alloc_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    int i = 0;
    
    for(i = 0;i < number;i++){
        pid = fork();
        if(pid < 0){
            printf("Error creating processes!\n");
            return 0;
        }
        if(pid == 0){
            proc_id = i + 1;
            printf("process %d starts!\n", getpid());
            break;
        }
    }

    if(i == number){
         printf("father is %d\n", getpid());
         proc_id = number;
    }

    if(syscall(361, getpid())){
        printf("Error on starting!\n");
        return 0;
    }

    for(i = 0;i < proc_id * proc_id * proc_id;i++){
			mprotect(memory, alloc_size, PROT_READ);
			memory[i] = i;
	}
    syscall(363, getpid(), &wcounts);
    printf("Task pid : %d, Wcount = %lu, times = %d\n", getpid(), wcounts, proc_id * 256);
	
	struct sched_param params;
	params.sched_priority = 0;
	if(sched_setscheduler(getpid(), 6, &params)){
			printf("failed to get scheduler!\n");
	}
	munmap(memory, alloc_size);
	int k;
    while(1);
	pid_t wpid;

    int status = 0;
    if(proc_id == number) while ((wpid = wait(&status)) > 0);
    printf("Process %d returns\n", proc_id);
    syscall(362, getpid());
    return 0;
}

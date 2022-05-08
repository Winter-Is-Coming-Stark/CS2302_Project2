#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int alloc_size;
static char* memory;
static int times;
int pid_1;
int pid_2;
int process_id;
void segv_handler(int signal_number)
{
    printf("find memory accessed!\n");
    mprotect(memory, alloc_size, PROT_READ | PROT_WRITE);
    times++;
    printf("set memory read write!\n");
}

unsigned long wcount;
void process1();
void process2();
void process3();

int main()
{
	pid_2 = -1;
    int fd;
    struct sigaction sa;

    printf("Start memory trace testing program!\n");

	pid_1 = fork();
	if(pid_1 < 0){
		fprintf(stderr, "Fork Failed\n");
		return 0;
	}
	else if(pid_1 > 0){
		pid_2 = fork();
		if(pid_2 < 0){
 			fprintf(stderr, "Fork Failed\n");
			return 0;
		}
	}

	if(pid_1 > 0 && pid_2 > 0) process_id = 1;
	else if (pid_1 == 0 && pid_2 < 0) process_id = 2;
	else process_id = 3;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &segv_handler;
    sigaction(SIGSEGV, &sa, NULL);

    times = 0;

    alloc_size = 10 * getpagesize();
    fd = open("/dev/zero", O_RDONLY);
    memory = mmap(NULL, alloc_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

	if(process_id == 3){
		process3();
	}
	if(process_id == 2){
		process2();
	}
	if(process_id == 1){
		int status1 = 0;
		int status2 = 0;
		if(waitpid(pid_1, &status1, 0) < 0){
			fprintf(stderr, "Error waiting pid\n");
			return 0;
		}
		if(!WIFEXITED(status1)){
			printf("waitpid returns error");
		}
		if(waitpid(pid_2, &status2, 0) < 0){
			fprintf(stderr, "Error waiting pid\n");
			return 0;
		}
		if(!WIFEXITED(status2)){
			printf("waitpid returns error");
		}
		process1();
	}


    return 0;
}

void process1()
{
    if(syscall(361, getpid())){
        printf("Error on starting!\n");
        return;
    }

    memory[0] = 0;
    printf("Task pid: %d,memory[0] = %d\n", getpid(), memory[0]);

    mprotect(memory, alloc_size, PROT_READ);
    memory[1] = 1;
    printf("Task pid: %d,memory[1] = %d\n", getpid(), memory[1]);

    syscall(363, getpid(), &wcount);
    printf("Task pid : %d, Wcount = %lu, times = %d\n", getpid(), wcount, times);
    munmap(memory, alloc_size);
    syscall(362, getpid());
}

void process2()
{
    if(syscall(361, getpid())){
        printf("Error on starting!\n");
        return;
    }
    memory[0] = 2;
	printf("Task pid: %d,memory[0] = %d\n", getpid(), memory[0]);

    mprotect(memory, alloc_size, PROT_READ);
    memory[1] = 3;
    printf("Task pid: %d,memory[1] = %d\n", getpid(), memory[1]);

    syscall(363, getpid(), &wcount);
    printf("Task pid : %d, Wcount = %lu, times = %d\n", getpid(), wcount, times);
    munmap(memory, alloc_size);
    syscall(362, getpid());
}

void process3()
{
    if(syscall(361, getpid())){
        printf("Error on starting!\n");
        return;
    }
    memory[0] = 4;
    printf("Task pid: %d,memory[0] = %d\n", getpid(), memory[0]);

    mprotect(memory, alloc_size, PROT_READ);
    memory[1] = 5;
    printf("Task pid: %d,memory[1] = %d\n", getpid(), memory[1]);

    syscall(363, getpid(), &wcount);
    printf("Task pid : %d, Wcount = %lu, times = %d\n", getpid(), wcount, times);
}

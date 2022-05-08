#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int alloc_size;
static char* memory;
static int times;

void segv_handler(int signal_number)
{
    printf("find memory accessed!\n");
    mprotect(memory, alloc_size, PROT_READ | PROT_WRITE);
    times++;
    printf("set memory read write!\n");
}

int main()
{
    int fd;
    struct sigaction sa;
    int wcount = 0;

    printf("Start memory trace testing program!\n");

    if(syscall(361, getpid())){
        printf("Error on starting!\n");
        return 0;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &segv_handler;
    sigaction(SIGSEGV, &sa, NULL);

    times = 0;

    alloc_size = 10 * getpagesize();
    fd = open("/dev/zero", O_RDONLY);
    memory = mmap(NULL, alloc_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    memory[0] = 0;
    printf("memory[0] = %d\n", memory[0]);

    mprotect(memory, alloc_size, PROT_READ);
    memory[0] = 1;
    printf("memory[0] = %d\n", memory[0]);

    syscall(363, getpid(), &wcount);
    printf("Task pid : %d, Wcount = %d, times = %d\n", getpid(), wcount, times);
    munmap(memory, alloc_size);
    syscall(362, getpid());
    return 0;
}

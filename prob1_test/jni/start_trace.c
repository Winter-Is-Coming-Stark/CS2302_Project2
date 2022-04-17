#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
		printf("This is a test:\n\n");
		syscall(361,getpid());
		syscall(361,getpid());
		printf("%d", getpid());
		syscall(362, getpid());
		return 0;
}

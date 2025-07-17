#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#define COUNT ((uint16_t)(255U))
#define IS_SYSCALL_FAILED(arg)   ( arg < 0 )
#define IS_NULLPTR(ptr)          (ptr == NULL)
#define LOG_ERROR(msg) \
        fprintf(stderr,"%s,  error code: %d ,  %s\n", msg, errno, strerror(errno));


int main(int argc, char *argv[]) {
    // Write your code here
    // Do not write a main() function. Instead, deal with echo_main() as the main function of your program.
    if(argc > 1){
	printf("Error using the utility %s\n", argv[0]);
	printf("Usage: %s\n", argv[0]);
	exit(-2);
    }
    // char buffer[COUNT];
    // char*cwd = getcwd(buffer, COUNT);
    char*cwd = getcwd(NULL, PATH_MAX+1);
    if(IS_NULLPTR(cwd))
    {
	    LOG_ERROR("Null ptr returned");
	    exit(-1);
    }else{
    	if(IS_SYSCALL_FAILED(write(1, cwd, strlen(cwd)))){
	   	 LOG_ERROR("Can not write to the stdout");
    	}
    	if(IS_SYSCALL_FAILED(write(1, "\n", 1))){
	    	LOG_ERROR("Can not write to the stdout");
    	}
    	//if(IS_NULLPTR(buffer)){
    		free(cwd);
    	//}
    }
    return 0;
}


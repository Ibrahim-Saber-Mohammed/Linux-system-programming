#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define COUNT ((uint16_t)(100U))
#define IS_SYSCALL_FAILED(arg)   ( arg < 0 )
#define LOG_ERROR(msg) \
        fprintf(stderr,"%s,  error code: %d ,  %s\n", msg, errno, strerror(errno));


int main(int argc, char *argv[]) {
    // Write your code here
    // Do not write a main() function. Instead, deal with echo_main() as the main function of your program.
    if(argc == 1){
        if(IS_SYSCALL_FAILED(write(1, "\n", 1))){
            LOG_ERROR("Error Writing to the stdout");
            exit(-2);
        }
        return 0;
    }
    int opt_counts=0;
    for(opt_counts=1; opt_counts<argc; opt_counts++){
        int bytes_nums = strlen(argv[opt_counts]);
        if(IS_SYSCALL_FAILED(write(1, argv[opt_counts], bytes_nums))){
                LOG_ERROR("Error Writing to the stdout");
                exit(-2);
        }
        if(opt_counts < (argc-1)){
            if(IS_SYSCALL_FAILED(write(1, " ", 1))){
                    LOG_ERROR("Error Writing to the stdout");
                    exit(-2);
            }
        }
    }
    if(IS_SYSCALL_FAILED(write(1, "\n", 1))){
        LOG_ERROR("Error Writing to the stdout");
        exit(-2);
    }
    return 0;

}

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


int  main(int argc, char* argv[]){
	if(argc < 2){
		printf("Error Using the utility\n");
		printf("Usage: %s <file-name>\n", argv[0]);
		printf("Example: %s /etc/passwd\n", argv[0]);
		exit(-3);
	}
	int opt_counts = 0;
	for(opt_counts=1; opt_counts<argc; opt_counts++){
		char buffer[COUNT];
		int fd = open(argv[opt_counts], O_RDONLY);
		if(IS_SYSCALL_FAILED(fd)){
			LOG_ERROR("Failed to open the file");
			exit(-1);
		}
		int bytes_nums ;
      		while((bytes_nums = read(fd, buffer, COUNT)) != 0){
			if(IS_SYSCALL_FAILED(write(1, buffer, bytes_nums))){
				LOG_ERROR("Error Writing to the stdout");
				exit(-2);
			}
       		}
	}
	return 0;
}

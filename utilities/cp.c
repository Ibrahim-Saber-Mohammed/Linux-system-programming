#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define STDOUT			 		((uint8_t)(1U))
#define BUF_SIZE       		 		((ssize_t)(1024U))
#define IS_SYSCALL_FAILED(arg)   		(arg < 0)
#define IS_NULLPTR(ptr)          		(ptr == NULL)
#define IS_WRITE_FAILED(size, target)		(target != size)
#define IS_READ_FAILED(size)			(size <= 0)

#define LOG_ERROR(msg) \
        fprintf(stderr,"%s,  error code: %d ,  %s\n", msg, errno, strerror(errno));

int  main(int argc, char* argv[]){
	int src_fd = 0;
	int dest_fd = 0;
	int dest_flags = 0;
	mode_t dest_perms = 0;
	ssize_t num_read;
    	char buffer[BUF_SIZE];

        if( (argc != 3) || ( (argc == 2) && (strcmp(argv[1], "--help") == 0))){
                printf("incorrect number of arguments to the utility\n");
		if( (argc == 2) && (strcmp(argv[1], "--help") == 0)){
                	printf("Usage: %s <src>  <destination>\n", argv[0]);
			exit(EXIT_SUCCESS);
		}
                exit(EXIT_FAILURE);
        }
	// open the source file as read only
	src_fd = open(argv[1], O_RDONLY);
	// check if the file is open successfuly	
	if(IS_SYSCALL_FAILED(src_fd)){
                LOG_ERROR("Failed to open the src file");
                exit(EXIT_FAILURE);
	}
	
	// destination file 
	dest_flags = O_WRONLY | O_TRUNC | O_CREAT;
	// rw-rw-rw
	dest_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; 
	dest_fd = open(argv[2], dest_flags, dest_perms);
       
       	// check if the file is open successfuly
        if(IS_SYSCALL_FAILED(dest_fd)){
		LOG_ERROR("Failed to open/create the dest file");
		close(src_fd);
		exit(EXIT_FAILURE);
	}
	// if file is open then read all the content and write it to the dest.
	while(!IS_READ_FAILED((num_read = read(src_fd, buffer, BUF_SIZE)))){
		if(IS_WRITE_FAILED(write(dest_fd, buffer, num_read), num_read))
		{
			LOG_ERROR("Could not write the buffer");
			close(src_fd);
			close(dest_fd);
                        exit(EXIT_FAILURE);
		}
	}
	if (IS_SYSCALL_FAILED(num_read)){
        	LOG_ERROR("read error");
		close(src_fd);
		close(dest_fd);
		exit(EXIT_FAILURE);
	}
	if(IS_SYSCALL_FAILED(close(src_fd))){
		LOG_ERROR("Could not close the src file");
	}
	if(IS_SYSCALL_FAILED(close(dest_fd))){
                LOG_ERROR("Could not close the dest file");
        }
	return EXIT_SUCCESS;
}

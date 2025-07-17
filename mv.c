#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#define BUF_SIZE				((ssize_t)(2048U))
#define IS_SYSCALL_FAILED(arg) 			(arg < 0)
#define IS_READ_FAILED(counts) 			(counts <= 0)
#define IS_WRITE_FAILED(counts, target)		(counts != target)

#define LOG_ERROR(msg) \
	fprintf(stderr, "%s, error code: %d - %s\n", msg, errno, strerror(errno));

int main(int argc, char* argv[])
{
	int src_fd = 0;
	int dest_fd = 0;
	int dest_flags = 0;
	mode_t dest_perms = 0;
	char buffer[BUF_SIZE];
	ssize_t bytes_read = 0;

	if(argc != 3 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        	printf("Usage : %s <src_filename> <dest_filename>\n", argv[0]);
       		// No error log for help, just print usage and exit gracefully
        	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
            		exit(EXIT_SUCCESS);
        	}
        	LOG_ERROR("Incorrect number of arguments or missing file names");
        	exit(EXIT_FAILURE);
	}

	src_fd = open(argv[1], O_RDONLY);
	if(IS_SYSCALL_FAILED(src_fd)){
		LOG_ERROR("Could not open the src file");
		exit(EXIT_FAILURE);
	}
	
	dest_flags = O_WRONLY | O_CREAT | O_TRUNC;
	dest_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	dest_fd = open(argv[2], dest_flags, dest_perms);
	if(IS_SYSCALL_FAILED(dest_fd)){
		LOG_ERROR("Could not open/create dest file");
		close(src_fd);
		exit(EXIT_FAILURE);
	}
	// if file is open then read all the content and write it to the dest.
	while(!IS_READ_FAILED((bytes_read = read(src_fd, buffer, BUF_SIZE)))){
		if(IS_WRITE_FAILED(write(dest_fd, buffer, bytes_read), bytes_read))
		{
			LOG_ERROR("Could not write the buffer");
                        close(src_fd);
			close(dest_fd);
			exit(EXIT_FAILURE);
		}
	}
	// error during the read
	if (IS_SYSCALL_FAILED(bytes_read)){
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
	if (IS_SYSCALL_FAILED(unlink(argv[1]))) {
        	LOG_ERROR("Could not remove the source file");
        	return EXIT_FAILURE; // Indicate partial success/failure
    	}

	return EXIT_SUCCESS;
}

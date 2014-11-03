#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

/* Fork test with tracker change
int main(void)
{
	
	pid_t pid;
	int rv;
	int tracker = 5;

	switch(pid = fork()) {
	case -1:
		perror("fork");
		exit(1);
	case 0:
		printf("CHILD: This is the child process!\n");
		printf("CHILD: My PID is %d.\n", getpid());
		printf("CHILD: My parent's PID is %d.\n", getppid());
		printf("CHILD: My tracker is %d.\n", tracker);
		printf(" CHILD: Enter my exit status (make it small): ");
       		scanf(" %d", &rv);
        	printf(" CHILD: I'm outta here!\n");
        	exit(rv);
	default:
       		printf("PARENT: This is the parent process!\n");
        	printf("PARENT: My PID is %d\n", getpid());
        	printf("PARENT: My child's PID is %d\n", pid);
		printf("PARENT: My tracker is %d.\n", tracker);
        	printf("PARENT: I'm now waiting for my child to exit()...\n");
       		wait(&rv);
		tracker = WEXITSTATUS(rv);
		printf("PARENT: My tracker is now %d.\n", tracker);
        	printf("PARENT: My child's exit status is: %d\n", WEXITSTATUS(rv));
        	printf("PARENT: I'm outta here!\n");
	}
	return 0;
}

* Use child_process(), parent_process() to separate the two.

*/

/*
int main(void)
{
	int i;
    	int fd;
   	int result;
   	int *map;  *//* mmapped array of int's */

    /* Open a file for writing.
     *  - Creating the file if it doesn't exist.
     *  - Truncating it to 0 size if it already exists. (not really needed)
     *
     * Note: "O_WRONLY" mode is not sufficient when mmaping.
     */
	/*
    fd = open("test", O_RDWR);
    if (fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    *//* Stretch the file size to the size of the (mmapped) array of ints
     */
	/*
    result = lseek(fd, FILESIZE-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
	exit(EXIT_FAILURE);
    }
    */
    /* Something needs to be written at the end of the file to
     * have the file actually have the new size.
     * Just writing an empty string at the current file position will do.
     *
     * Note:
     *  - The current position in the file is at the end of the stretched 
     *    file due to the call to lseek().
     *  - An empty string is actually a single '\0' character, so a zero-byte
*/


#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int)) 

int main(void)
{
	int fd;
	char* map;

	fd = open("test", O_RDWR | O_CREAT);
	//File must be stretched to a suitable size for the mmap array.
	lseek(fd, FILESIZE-1, SEEK_SET);
	//Something must be written to end of file to give it its new size.
	write(fd, "", 1);
	map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	pid_t pid;
	int rv;
	int val1, val2, val3;
	
	switch(pid = fork()) {
	case -1:
		perror("fork() has not worked!");
		exit(1);
	case 0:
		printf("CHILD: I am the child!\n");
		printf("CHILD: My PID is %d.\n", getpid());
		printf("CHILD: First value for mmap?\n");
		scanf(" %d", &val1);
		printf("CHILD: Second value for mmap?\n");
		scanf(" %d", &val2);
		printf("CHILD: Third value for mmap?\n");
		scanf(" %d", &val3);
		map[0] = val1;
		map[1] = val2;
		map[2] = val3;
		printf("CHILD: Enter my return status: ");
		scanf(" %d", &rv);
		printf("CHILD: I'm done!\n");
		break;
	default:
		printf("PARENT: I am the parent!\n");
		printf("PARENT: My PID is %d.\n", getpid());
		wait(&rv);
		printf("PARENT: My child's return value was %d.\n", rv);
		printf("PARENT: My child set the following values:\n");
		printf("PARENT: map[0] is %d.\n", map[0]);
		printf("PARENT: map[1] is %d.\n", map[1]);
		printf("PARENT: map[2] is %d.\n", map[2]);
		printf("PARENT: I'm done!\n");
        break;
	}
	
	return 0;
}

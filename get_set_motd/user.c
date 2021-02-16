#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/cust_motd"

int main(int argc, char *argv[])
{
	int fd, ret;
	char message[50];
	char *change = "Hello Kernel!";

	fd = open(DEVICE, O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}

	ret = read(fd, message, 50);
	printf("Message read: %s\n", message);
	printf("Bytes read: %d\n", ret);


	ret = write(fd, change, strlen(change));
	if(ret>0)
	{
		printf("MOTD has changed\n");
	}
	ret = read(fd, message, 50);
	printf("New MOTD: %s\n", message);


	close(fd);

	return 0;
}

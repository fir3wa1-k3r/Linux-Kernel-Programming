#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd;
	int temp, ch;
	char a[10] = "";
	//open the file

	fd = open("/dev/cccc", O_RDWR);

	if(fd<0)	
	{
		printf("Unable to open the file /dev/cccc\n");
	}
	while (1)
	{
		printf("Enter 1 to read, 2 to write 3 to exit: ");
		scanf("%d", &ch);
		
		switch(ch)
		{
			case 1:
				//read the file
				temp = read(fd, a, 10);
				break;
			case 2:
				//write to file
				temp = write(fd, a, 1 );
				break;
			default:
				exit(1);
		}
	}
	close(fd);
	
	return 0;
}

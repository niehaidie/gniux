#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd_s,fd_d;
	char buf[512];

	if(argc != 3)
	{
		printf("Usage: %s sourcefile distfile\n", argv[0]);
		return 1;
	}
	if((fd_s = open(argv[1], O_RDONLY)) < 0)	
	{
		printf("open file %s error\n", argv[1]);
		return 1;
	}
	if((fd_d = open(argv[2], O_RDWR)) < 0)	
	{
		printf("open file %s error\n", argv[2]);
		close(fd_s);
		return 1;
	}

	while(read(fd_s, buf, 512))
	{
		write(fd_d, buf, 512);
	}

	close(fd_d);
	close(fd_s);

	return 0;
}

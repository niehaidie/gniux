#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	struct stat stat;
	int f1_size, f2_size;
	int fd1, fd2;
	int i, j;
	unsigned char *buf1, *buf2;

	if(argc != 3)
	{
		printf("Usage: %s filename1 filename2\n", argv[0]);
		return -1;
	}

	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);

	if(fstat(fd1, &stat))
	{
		printf("fstat error\n");
		return -1;
	}
	f1_size = stat.st_size;
	
	if(fstat(fd2, &stat))
	{
		printf("fstat error\n");
		return -1;
	}
	f2_size = stat.st_size;

	buf1 = (unsigned char *)malloc(2);
	buf2 = (unsigned char *)malloc(2);
	
	for(i = 0; i < ((f1_size < f2_size) ? f1_size : f2_size); i++)
	{
		if(i%16 == 0)
			printf("\n%8x: ", i);
		read(fd1, buf1, 1);
		read(fd2, buf2, 1);
		if(*buf1 == *buf2)
			printf("%02x ", *buf1);
		else
		{
			printf("\ndiffer:\n");
			printf("%s\t:%02x\n", argv[1], *buf1);
			printf("%s\t:%02x\n", argv[2], *buf2);
			return 1;
		}
	}
	if(f1_size < f2_size)
	{
		printf("\n%s:\n", argv[2]);
		for(j = 0; j < i%16; j++)
			printf("   ");
		for(i; i < f2_size; i++)
		{
			read(fd2, buf2, 1);
			printf("%02x ", *buf2);
		}
	}
	else if(f1_size > f2_size)
	{
		printf("\n%s:\n", argv[1]);
		for(j = 0; j < i%16; j++)
			printf("   ");
		for(i; i < f1_size; i++)
		{
			read(fd1, buf1, 1);
			printf("%02x ", *buf1);
		}
	}
	else
	{
		printf("\n%s and %s are same!\n", argv[1], argv[2]);
	}

	return 0;
}

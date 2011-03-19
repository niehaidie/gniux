#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	struct stat stat;
	int f1_size;
	int fd1;
	int i, j;
	unsigned char *buf1;

	if(argc != 2)
	{
		printf("Usage: %s filename\n", argv[0]);
		return -1;
	}

	fd1 = open(argv[1], O_RDONLY);

	if(fstat(fd1, &stat))
	{
		printf("fstat error\n");
		return -1;
	}
	f1_size = stat.st_size;
	printf("filesize = %d(0x%x)\n", f1_size, f1_size);	
	buf1 = (unsigned char *)malloc(2);
	
	for(i = 0; i < f1_size; i++)
	{
		if(i%16 == 0)
			printf("\n%8x: ", i);
		read(fd1, buf1, 1);
		printf("%02x ", *buf1);
	}
	printf("\n");

	return 0;
}

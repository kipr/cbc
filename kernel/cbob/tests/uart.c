#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fd;
	char data[256];
	int count, i;

	fd = open("/dev/cbc/uart0", O_RDONLY);

	while(1) {
		count = read(fd, &data, 256);
		if(count > 0)
			printf("Got count=%d\n", count);
		for(i = 0;count > 0 && i < count;i++) {
			putchar(data[count]);
		}
		sleep(1);
	}

	close(fd);
}

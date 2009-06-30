#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int servo, fd, error;
	char *servo_dev;
	short position;
	
	if(argc < 3) {
		printf("Usage: %s <servo> <position>\n", argv[0]);
		return 1;
	}

	servo = atoi(argv[1]);
	position = (short)atoi(argv[2]);

	if(servo < 0 || servo > 3) {
		printf("Invalid servo port: must be 0..3\n");
		return 1;
	}

  if(position < -1 || position > 2048) {
		printf("Invalid servo value: must be from -1..2048\n");
		return 1;
	}

	switch(servo) {
		case 0:
			servo_dev = "/dev/cbc/servo0";
			break;
		case 1:
			servo_dev = "/dev/cbc/servo1";
			break;
		case 2:
			servo_dev = "/dev/cbc/servo2";
			break;
		case 3:
			servo_dev = "/dev/cbc/servo3";
			break;
	}

	fd = open(servo_dev, O_WRONLY);
	
	if(fd < 0) {
		printf("Error opening servo %d: %s\n", servo, strerror(fd));
		return 1;
	}

	printf("Setting servo %d to position: %hd\n", servo, position);
	error = write(fd, &position, 2);
	
	if(error < 0) {
		printf("Error writing to servo %d: %s\n", servo, strerror(error));
		return 1;
	}

	close(fd);
}

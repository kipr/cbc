#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int motor, fd, error;
	char *motor_dev;
	signed char pwm;
	
	if(argc < 3) {
		printf("Usage: %s <motor> <power>\n", argv[0]);
		return 1;
	}

	motor = atoi(argv[1]);
	pwm = (signed char)atoi(argv[2]);

	if(motor < 0 || motor > 3) {
		printf("Invalid motor port: must be 0..3\n");
		return 1;
	}

	if(pwm < -100 || pwm > 100) {
		printf("Invalid pwm value: must be from -100..100\n");
		return 1;
	}
	
	switch(motor) {
		case 0:
			motor_dev = "/dev/cbc/pwm0";
			break;
		case 1:
			motor_dev = "/dev/cbc/pwm1";
			break;
		case 2:
			motor_dev = "/dev/cbc/pwm2";
			break;
		case 3:
			motor_dev = "/dev/cbc/pwm3";
			break;
	}

	fd = open(motor_dev, O_WRONLY);
	
	if(fd < 0) {
		printf("Error opening motor %d: %s\n", motor, strerror(fd));
		return 1;
	}

	printf("Setting motor %d to velocity: %hd\n", motor, velocity);
	error = write(fd, &pwm, 1);
	
	if(error < 0) {
		printf("Error writing to motor %d: %s\n", motor, strerror(error));
		return 1;
	}

	close(fd);
}

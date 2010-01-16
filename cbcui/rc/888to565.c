#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int round(float x)
{
	int integer = (int)x;
	float decimal = x - (float)integer;

	if(decimal < 0.5)
		return integer;
	else if(decimal > 0.5)
		return integer+1;
	else {
		if(integer%2)
			return integer + 1;
		else
			return integer;
	}
}

int main(int argc, char **argv)
{
	unsigned char rgb888[3];
	unsigned char rgb565[2];
	unsigned char red,green,blue;

	FILE *infile, *outfile;

	if(argc != 3) {
		printf("Incorrect usage...\n");
		printf("Try %s <infile> <outfile>\n", argv[0]);
		exit(1);
	}

	infile = fopen(argv[1], "r");

	if(infile == 0) {
		printf("Unable to open input file: %s for reading.\n");
		exit(1);
	}

	outfile = fopen(argv[2], "w+");

	if(outfile == 0) {
		printf("Unable to open output file: %s for writing.\n");
		fclose(infile);
		exit(1);
	}
	
	while(fread(rgb888, 1, 3, infile) == 3) {
		red = round(((float)rgb888[2])/8.0);
		green = round(((float)rgb888[1])/4.0);
		blue = round(((float)rgb888[0])/8.0);
		//red = rgb888[2] >> 3;
		//green = rgb888[1] >> 2;
		//blue = rgb888[0] >> 3;

		rgb565[0] = red | (green<<5);
		rgb565[1] = (green>>3) | (blue<<3);

		fwrite(rgb565, 1, 2, outfile);
	}

	fclose(infile);
	fclose(outfile);

	return 0;
}


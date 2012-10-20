#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int setGpio (char pinNumber, char value)
{
	if (value > 1) return;
	char fileName[30];
	FILE * gpioWrite;
	strcat (fileName, "/sys/class/gpio/gpio");
	strcat (fileName, &pinNumber);
	strcat (fileName, "/value");
	if ((gpioWrite = fopen (fileName, "w")) == NULL) return -1;
	fputc (value, gpioWrite);
	fclose (gpioWrite);
}
int readGpio (char pinNumber)
{
	char fileName[30];
	FILE * gpioRead;
	strcat (fileName, "/sys/class/gpio/gpio");
	strcat (fileName, &pinNumber);
	strcat (fileName, "/value");
	if ((gpioRead = fopen (fileName, "r")) == NULL) return -1;
	int ret = fgetc (gpioRead);
	fclose (gpioRead);
	return ret;
}

int main ()
{
	int gpioIndex;
	char gpioId;
	char fileName[32];
	FILE * gpioInit;
	char * direction = "in\0";
	for (gpioIndex = 0, gpioId = '1'; gpioIndex < 26; gpioIndex++, gpioId++)
	{
		strcat (fileName, "/sys/class/gpio/export");
		gpioInit = fopen (fileName, "w");
		fputc (gpioId, gpioInit);
		fclose (gpioInit);
		strcat (fileName, "/sys/class/gpio/gpio");
		strcat (fileName, &gpioId);
		strcat (fileName, "/direction");
		gpioInit = fopen (fileName, "w");
		fputs (direction, gpioInit);
		fclose (gpioInit);
	}
	int serialFd = open ("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (serialFd == -1)
	{
		printf ("Error setting up serial port.");
		return -1;
	}
	getchar ();
	char serialIn[5];;
	read (serialFd, serialIn, 4);
	printf (serialIn);
	write (serialFd, "TEST\r", 5);
	setGpio ('7', '1');
	close (serialFd);
	return 0;
}

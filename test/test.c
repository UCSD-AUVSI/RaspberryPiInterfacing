#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
//#include <sys/type.h>
#include <sys/time.h>
#include <sys/select.h>

int setGpio (char value)
{
	if (value != '1' && value != '0') return;
	FILE * gpioWrite;
	FILE * gpioInit;
	gpioInit = fopen ("/sys/class/gpio/gpio7/direction", "w");
	fputs ("out", gpioInit);
	fclose (gpioInit);
	if ((gpioWrite = fopen ("/sys/class/gpio/gpio7/value", "w")) == NULL) return -1;
	fputc (value, gpioWrite);
	fclose (gpioWrite);
}
int readGpio ()
{
	FILE * gpioRead;
	FILE * gpioInit;
	gpioInit = fopen ("/sys/class/gpio/gpio7/direction", "w");
	fputs ("in", gpioInit);
	fclose (gpioInit);
	if ((gpioRead = fopen ("/sys/class/gpio/gpio7/value", "r")) == NULL) return -1;
	int ret = fgetc (gpioRead);
	fclose (gpioRead);
	return ret;
}

int main ()
{
	FILE * gpioInit;
	gpioInit = fopen ("/sys/class/gpio/export", "w");
	fputc ('7', gpioInit);
	fclose (gpioInit);
	gpioInit = fopen ("/sys/class/gpio/gpio7/direction", "w");
	fputs ("out", gpioInit);
	fclose (gpioInit);
	setGpio ('1');
	int serialFd = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (serialFd == -1)
	{
		perror ("Error setting up serial port - ");
		return -1;
	}
	struct termios options;
	tcgetattr(serialFd, &options);
	cfsetispeed(&options, B57600);
	cfsetospeed(&options, B57600);
	options.c_cflag |= (CLOCAL |CREAD);
	tcsetattr(serialFd, TCSANOW, &options);
	getchar ();
	write (serialFd, "TEST\r", 5);
	char data_in[50];
	struct timeval timeout;
	fd_set fdSet;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int max_fd;
	int n;
	while(read(serialFd, &data_in[0], sizeof(char))){
		 
		if(data_in[0] != '\0'){
			printf("%c\n",data_in[0]);
		}
		data_in[0] = '\0';
	}
	close (serialFd);
	return 0;
}

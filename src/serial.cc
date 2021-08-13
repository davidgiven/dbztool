#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/poll.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "globals.h"

static int fd;
static struct termios serialterm;

#include ".obj/stubs/fastmode_stub.h"
#include ".obj/stubs/ping_stub.h"

static int getbaudrate(int speed)
{
	switch (speed)
	{
		case 50:	return B50;
		case 75:	return B75;
		case 110:	return B110;
		case 134:	return B134;
		case 150:	return B150;
		case 200:	return B200;
		case 300:	return B300;
		case 600:	return B600;
		case 1200:	return B1200;
		case 1800:	return B1800;
		case 2400:	return B2400;
		case 4800:	return B4800;
		case 9600:	return B9600;
		case 19200:	return B19200;
		case 38400:	return B38400;
		case 57600:	return B57600;
		case 115200:	return B115200;
		case 230400:	return B230400;
	}

	error("%d baud is not supported", speed);
	return 0;
}

static bool pause(int milliseconds)
{
	struct pollfd p;
	p.fd = fd;
	p.events = POLLIN;
	return !!poll(&p, 1, milliseconds);
}

static void synchronise()
{
	int i;
	struct pollfd p;

	/* Set to the slow rate. */

	cfsetspeed(&serialterm, getbaudrate(SlowBaudRate));
	i = tcsetattr(fd, TCSANOW, &serialterm);
	if (i == -1)
		error("Failed to set up serial port.");

	unsigned char c;

	printf("Waiting for device reset...\n");

	for (;;)
	{
		/* Flush buffers. */

		for (;;)
		{
			i = read(fd, &c, 1);
			if (i != 1)
				break;
		}

		/* Write a @. */

		c = '@';
		i = write(fd, &c, 1);
		if (i == -1)
			error("Write error.");

		/* Wait for data; max on 100ms. */

		p.fd = fd;
		p.events = POLLIN;
		if (poll(&p, 1, 100) == 0)
			continue;

		/* We got some data. Read it and make sure it's a @. */

		c = 0;
		(void) read(fd, &c, 1);
		if (c == '@')
			break;
	}
	
	printf("Switching to fast mode...\n");

	std::string stub((char*)_obj_stubs_fastmode_bin, _obj_stubs_fastmode_bin_len);
	pad_with_nops(stub);
	brecord_write(0xffffffc0, stub.size(), (const uint8_t*) &stub[0]);
	brecord_execute(0xffffffc0);
	cfsetspeed(&serialterm, B19200);

	verbose("Pinging board\n");

	std::string pingStub((char*)_obj_stubs_ping_bin, _obj_stubs_ping_bin_len);
	pad_with_nops(pingStub);
	brecord_write(0xffffffc0, pingStub.size(), (const uint8_t*) &pingStub[0]);
	brecord_execute(0xffffffc0);
	
	c = recvbyte();
	if (c != 'P')
		error("Synchronisation error (got %02X instead of %02X).", c, 'P');
}

void logon()
{
	fd = open(SerialPort, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		error("Failed to open serial port.");

	/* Set up the serial port. */

	tcgetattr(fd, &serialterm);
	serialterm.c_cflag = CS8 | CLOCAL | CREAD;
	serialterm.c_lflag = 0;
	serialterm.c_oflag = 0;
	serialterm.c_iflag = IGNPAR;

	synchronise();
}

void sendbyte(uint8_t c)
{
#ifdef SPEW_TRACING
	printf("<%02X\n", c);
#endif

	int i;
	do
	{
		struct pollfd p;
		p.fd = fd;
		p.events = POLLOUT;
		if (poll(&p, 1, INT_MAX) == 0)
			break;

		i = write(fd, &c, 1);
		if (i == -1)
			error("I/O error on write: %s",
				strerror(errno));
	}
	while (i != 1);
}

uint8_t recvbyte()
{
	int i;
	uint8_t b;

	do
	{
		struct pollfd p;
		p.fd = fd;
		p.events = POLLIN;
		if (poll(&p, 1, INT_MAX) == 0)
			break;

		i = read(fd, &b, 1);
		if (i == -1)
			error("I/O error on read: %s",
				strerror(errno));
	}
	while (i != 1);

#ifdef SPEW_TRACING
	printf(">%02X\n", b);
#endif
	return b;
}

void send(const std::string& s)
{
	for (char c : s)
	{
		sendbyte((uint8_t) c);
		if (recvbyte() != c)
			error("corrupt transfer");
	}
}

void dodgyterm()
{
	printf("Serial terminal starting (CTRL+C to quit)\n");

	/* Set up the serial port in slow mode. */

	fd = open(SerialPort, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		error("Failed to open serial port.");

	tcgetattr(fd, &serialterm);
	serialterm.c_cflag = CS8 | CLOCAL | CREAD;
	serialterm.c_lflag = 0;
	serialterm.c_oflag = 0;
	serialterm.c_iflag = IGNPAR;

	cfsetspeed(&serialterm, getbaudrate(SlowBaudRate));
	int i = tcsetattr(fd, TCSADRAIN, &serialterm);
	if (i == -1)
		error("Failed to set up serial port.");

	/* Put the console into raw mode. */

	struct termios oldconsole;
	struct termios console;
	tcgetattr(0, &oldconsole);
	console = oldconsole;
	cfmakeraw(&console);
	i = tcsetattr(0, TCSANOW, &console);
	if (i == -1)
		error("Failed to put console into raw mode.");

	/* Wait for input on either device. */

	for (;;)
	{
		struct pollfd p[2];
		p[0].fd = fd;
		p[0].events = POLLIN;
		p[1].fd = 0;
		p[1].events = POLLIN;

		poll(p, 2, INT_MAX);

		if (p[0].revents)
		{
			uint8_t b = recvbyte();
			write(0, &b, 1);
		}

		if (p[1].revents)
		{
			uint8_t b;
			read(0, &b, 1);
			if (b == 3)
				break;
			sendbyte(b);
		}
	}

	/* Put the console back the way it was. */

	tcsetattr(0, TCSANOW, &oldconsole);
}


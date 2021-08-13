#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"

bool Verbose = false;
const char* SerialPort = "/dev/ttyS0";

static int argc;
static char** argv;

static void show_help()
{
	printf(
		"dbztool: Dragonball VZ bootstrap client v%s.\n"
		"(C) 2021 David Given.\n"
		"Usage: dbztool [<options>] <command...>\n"
		"Options:\n"
		"   -h         Displays this message\n"
		"   -v         Switch on verbose mode\n"
		"   -p <port>  Sets the serial port to use\n"
		"Commands:\n"
		"   ping       Pings the device, testing communication\n"
		"   term       Start simple serial terminal\n"
		"   execute <address>\n"
		"              Executes an on-device program and enters the terminal\n"
		"   read <filename> <startaddress> <length>\n"
		"              Reads data from RAM into the specified file\n"
		"   write <filename> <startaddress>\n"
		"              Writes data from the specified file into RAM\n"
		"   cs\n"
		"              Reads and displays the chip select register status\n"
		"Addresses should begin 0x if you want them in hex.\n"
		"WARNING. Use this program at your own risk. The author accepts no responsibility\n"
		"    for any damage this program may do to your hardware. You have been warned!\n",
		VERSION
	);
}

static void parse_options()
{
	for (;;)
	{
		int c = getopt(argc, argv,
			":hvre:p:f:s:m:");
		switch (c)
		{
			case -1:
				argv += optind;
				return;

			case 'h':
				show_help();
				exit(0);

			case 'v':
				Verbose = true;
				break;

			case 'p':
				SerialPort = optarg;
				break;

			case ':':
				error("missing option argument.");

			default:
				error("try '-h' for a usage summary.");
		}
	}
}

int main(int _argc, char* _argv[])
{
	argc = _argc;
	argv = _argv;
	parse_options();

	const char* cmd = argv[0];
	if (!cmd)
		error("nothing to do! Try '-h' for a usage summary.");

	argv++;
	if (strcmp(cmd, "ping") == 0)
		logon();
	else if (strcmp(cmd, "term") == 0)
		dodgyterm();
	else if (strcmp(cmd, "execute") == 0)
	{
		logon();
		cmd_execute(argv);
	}
	else if (strcmp(cmd, "read") == 0)
	{
		logon();
		cmd_read(argv);
	}
	else if (strcmp(cmd, "write") == 0)
	{
		logon();
		cmd_write(argv);
	}
	else if (strcmp(cmd, "cs") == 0)
	{
		logon();
		cmd_cs(argv);
	}
	else
		error("unrecognised command! Try '-h' for a usage summary.");

	return 0;
};


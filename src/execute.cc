#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"

/* --- Execute code ------------------------------------------------------ */

static void exec_execute(uint32_t address)
{
	brecord_execute(address);
};

void cmd_execute(char** argv)
{
	const char* start = argv[0];

	if (!start || argv[1])
		error("syntax error: execute <start>");

	int64_t s = strtoll(start, NULL, 0);
	if ((s < 0) || (s > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	exec_execute(s);
};



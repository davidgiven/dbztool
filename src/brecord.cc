#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"

void brecord_execute(uint32_t addr)
{
	send(aprintf("\n%08X00", addr));
}

void brecord_write(uint32_t addr, uint8_t count, const uint8_t* data)
{
	if (count == 0)
		return;

	send(aprintf("\n%08X%02X", addr, count));
	for (int i=0; i<count; i++)
		send(aprintf("%02X", data[i]));
}



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"

#include ".obj/stubs/ping_stub.h"

void cmd_ping(char** argv)
{
	if (argv[0])
		error("syntax error: ping");

	Bytes stub(_obj_stubs_ping_bin, _obj_stubs_ping_bin_len);
	pad_with_nops(stub);
	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);
	
	uint8_t c = recvbyte();
	if (c != 'P')
		error("Synchronisation error (got %02X instead of %02X).", c, 'P');
}


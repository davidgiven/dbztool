#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"

#include ".obj/stubs/getsp_stub.h"

void cmd_getsp(char** argv)
{
	if (argv[0])
		error("syntax error: getsp");
	
	Bytes stub(_obj_stubs_getsp_bin, _obj_stubs_getsp_bin_len);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);

	Bytes data;
	data.resize(8);
	for (int i=0; i<8; i++)
		data[i] = recvbyte();

	printf("SP = %08x\n", readbe32(&data[0]));
	printf("USP = %08x\n", readbe32(&data[4]));
}




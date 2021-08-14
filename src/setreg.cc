#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"

#include ".obj/stubs/writeregs_stub.h"

static void stop()
{
	sendbyte(0);
	sendbyte(0);
	sendbyte(0);
}

void cmd_setreg(char** argv)
{
	Bytes stub(_obj_stubs_writeregs_bin, _obj_stubs_writeregs_bin_len);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);

	int arg = 0;
	while (argv[arg])
	{
		if (!argv[arg+1])
		{
			stop();
			error("syntax error: setreg [<name> <value>...]");
		}

		const char* name = argv[arg];
		uint32_t value = strtoul(argv[arg+1], nullptr, 0);
		arg += 2;

		const Register* reg = find_register(name);
		if (!reg)
		{
			stop();
			error("unknown register name");
		}

		printf("Setting %s at %08x to ", name, reg->address);

		Bytes data;
		data.resize(reg->width);
		switch (reg->width)
		{
			case 1: data[0] = value; printf("%02x", value); break;
			case 2: writebe16(&data[0], value); printf("%04x", value); break;
			case 4: writebe32(&data[0], value); printf("%08x", value); break;
		}

		for (int i=0; i<reg->width; i++)
		{
			uint32_t address = reg->address + i;
			sendbyte(data[i]);
			sendbyte(address>>8);
			sendbyte(address);
		}

		putchar('\n');
		fflush(stdout);
	}

	stop();
}


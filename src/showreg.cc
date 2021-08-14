#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"

#include ".obj/stubs/readregs_stub.h"

void cmd_showreg(char** argv)
{
	if (!argv[0])
		error("syntax error: showreg [<name pattern>...]");
	
	Bytes stub(_obj_stubs_readregs_bin, _obj_stubs_readregs_bin_len);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);

	while (*argv)
	{
		const char* pattern = *argv++;
		foreach_register(pattern, [&](const std::string& name, const Register& reg)
		{
			printf("Register %s = ", name.c_str());
			fflush(stdout);

			Bytes data;
			data.resize(reg.width);

			uint32_t address = reg.address;
			for (int i=0; i<reg.width; i++)
			{
				sendbyte(address>>8);
				sendbyte(address);
				address++;
				data[i] = recvbyte();
			}

			switch (reg.width)
			{
				case 1: printf("%02x\n", data[0]); break;
				case 2: printf("%04x\n", readbe16(&data[0])); break;
				case 4: printf("%08x\n", readbe32(&data[0])); break;
			}

			return false;
		});
	}

	sendbyte(0);
	sendbyte(0);
}



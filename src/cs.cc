#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"
#include <map>

static const char* size_tab[] = {
	"32kB", "64kB", "128kB", "256kB", "512kB", "1MB", "2MB", "4MB", "8MB", "16MB"
};

static const std::map<std::string, uint32_t> cs_registers = {
	{ "CSGBA", CSGBA },
	{ "CSGBB", CSGBB },
	{ "CSGBC", CSGBC },
	{ "CSGBD", CSGBD },
	{ "CSA", CSA },
	{ "CSB", CSB },
	{ "CSC", CSC },
	{ "CSD", CSD },
	{ "CSUGBA", CSUGBA },
	{ "CEMUCS", EMUCS },
	{ "CSCTRL1", CSCTRL1 },
	{ "CSCTRL2", CSCTRL2 },
};

void cmd_cs(char** argv)
{
	if (!argv[0])
	{
		Bytes data = buffer_read(CSGBA, 0x20);
		auto showreg = [&](uint32_t address, const char* name)
		{
			int offset = address - CSGBA;
			uint16_t value = readbe16(&data[offset]);
			printf("%s = %04x\n", name, value);
			return value;
		};

		printf("\nRegisters:\n\n");

		uint16_t csgba = showreg(CSGBA, "CSGBA");
		uint16_t csgbb = showreg(CSGBB, "CSGBB");
		uint16_t csgbc = showreg(CSGBC, "CSGBC");
		uint16_t csgbd = showreg(CSGBD, "CSGBD");
		uint16_t csugba = showreg(CSUGBA, "CSUGBA");
		uint16_t csa = showreg(CSA, "CSA");
		uint16_t csb = showreg(CSB, "CSB");
		uint16_t csc = showreg(CSC, "CSC");
		uint16_t csd = showreg(CSD, "CSD");
		uint16_t emucs = showreg(EMUCS, "EMUCS");
		uint16_t csctrl1 = showreg(CSCTRL1, "CSCTRL1");
		uint16_t csctrl2 = showreg(CSCTRL2, "CSCTRL2");

		auto showregion = [&](uint16_t cs, uint16_t csgb, int which, const char* name)
		{
			int siz = (cs & 0x000e) >> 1;
			if (which <= 1)
				siz += 2;

			uint32_t base = csgb << 14;
			if (csugba & 0x8000)
				base |= (csugba << (1 + which*4)) & 0xe0000000;

			int ws = ((cs & 0x0030) >> 3) | ((csctrl1 >> (8 + which)) & 1);

			printf("%s: %08x+%s %dws %s%s%s%s%s%s%s\n", name,
				base,
				size_tab[siz],
				ws,
				(cs & 0x8000) ? "RO " : "",
				(cs & 0x4000) ? "SOP " : "",
				(cs & 0x2000) ? "ROP " : "",
				(cs & 0x0200) ? "DRAM " : "",
				(cs & 0x0100) ? "F " : "",
				(cs & 0x0080) ? "16bit " : "8bit ",
				(cs & 0x0001) ? "ENABLED" : "!enabled");
		};

		printf("\nSummary:\n\n");
		showregion(csa, csgba, 0, "A");
		showregion(csb, csgbb, 1, "B");
		showregion(csc, csgbc, 2, "C");
		showregion(csd, csgbd, 3, "D");
	}
	else
	{
		int arg = 0;
		while (argv[arg])
		{
			if (!argv[arg+1])
				error("syntax error: cs [<name> <value>...]");

			const char* name = argv[arg];
			uint16_t value = strtoul(argv[arg+1], nullptr, 0);
			arg += 2;

			const Register* reg = find_register(name);
			if (!reg)
				error("unknown register name");

			printf("setting %s at %08x to %04x\n", name, reg->address, value);
			Bytes data;
			data.resize(2);
			writebe16(&data[0], value);

			brecord_write(reg->address, 2, &data[0]);
		}
	}
};



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include <algorithm>

using std::min;

#include ".obj/stubs/write_stub.h"

/* --- Write RAM --------------------------------------------------------- */

static void exec_write(const char* filename, uint32_t start)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		error("Could not open input file: %s", strerror(errno));

	fseek(fp, 0, SEEK_END);
	uint32_t length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("Writing '%s' to RAM at address 0x%08X+0x%08X:\n", filename, start, length);

	std::string stub((char*)_obj_stubs_write_bin, _obj_stubs_write_bin_len);
	writebe((uint8_t*) &stub[2], start);
	writebe((uint8_t*) &stub[8], length);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), (const uint8_t*) &stub[0]);
	brecord_execute(0xffffffc0);

	uint32_t count = 0;
	auto progress = [&]() {
		printf("\r% 3.0f%% complete: %d bytes (%.0f Bps)",
				(100.0*count) / length,
				count, count / gettime());
	};

	resettimer();
	while (count < length)
	{
		uint8_t b = fgetc(fp);
		sendbyte(b);

		count++;
		if ((count & 0xff) == 0)
		{
			progress();
			fflush(stdout);
		}
	}
	progress();
	putchar('\n');

	fclose(fp);
};

void cmd_write(char** argv)
{
	const char* filename = argv[0];
	const char* start = filename ? argv[1] : NULL;

	if (!filename || !start || argv[2])
		error("syntax error: write <filename> <start>");

	int64_t s = strtoll(start, NULL, 0);
	if ((s < 0) || (s > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	exec_write(filename, s);
};


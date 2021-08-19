#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include <algorithm>

#include ".obj/stubs/write_stub.h"

/* Due to silicon errata 10, any write to memory in the range xxxxxxC0..DF will
 * also update the code fragment buffer, overwriting the code fragment. So, we
 * can't put our stub there. As the only location that we know is safe to write
 * to is the area actually being written to, we have to put the stub there,
 * which makes things a little awkward... */

static void exec_write(const char* filename, uint32_t start)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		error("Could not open input file: %s", strerror(errno));

	fseek(fp, 0, SEEK_END);
	uint32_t length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("Writing '%s' to RAM at address 0x%08X+0x%08X:\n", filename, start, length);

	uint32_t count = 0;
	auto progress = [&]() {
		printf("\r% 3.0f%% complete: %d bytes (%.0f Bps)",
				(100.0*count) / length,
				count, count / gettime());
	};

	resettimer();
	if (length > _obj_stubs_write_bin_len)
	{
		uint8_t prelude[_obj_stubs_write_bin_len];
		fread(prelude, 1, _obj_stubs_write_bin_len, fp);

		printf("(uploading stub)\n");
		Bytes stub(_obj_stubs_write_bin, _obj_stubs_write_bin_len);
		writebe32(&stub[2], start+_obj_stubs_write_bin_len);
		writebe32(&stub[8], length-_obj_stubs_write_bin_len);

		brecord_write(start, stub.size(), &stub[0]);
		brecord_execute(start);

		while (count < (length - _obj_stubs_write_bin_len))
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

		brecord_write(start, sizeof(prelude), prelude);
	}
	else
	{
		progress();

		uint8_t buffer[_obj_stubs_write_bin_len];
		int i = fread(buffer, 1, length, fp);
		brecord_write(start, length, buffer);
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


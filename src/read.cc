#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include <algorithm>

#include ".obj/stubs/read_stub.h"

/* --- Read RAM ---------------------------------------------------------- */

Bytes buffer_read(uint32_t start, uint32_t length)
{
	Bytes stub(_obj_stubs_read_bin, _obj_stubs_read_bin_len);
	writebe32(&stub[2], start);
	writebe32(&stub[8], length);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);

	Bytes bytes;
	uint32_t count = 0;
	while (count < length)
	{
		bytes += recvbyte();
		count++;
	}

	return bytes;
}

void exec_dump(uint32_t start, uint32_t length)
{
	Bytes stub(_obj_stubs_read_bin, _obj_stubs_read_bin_len);
	writebe32(&stub[2], start);
	writebe32(&stub[8], length);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);

	Bytes bytes;
	uint32_t count = 0;

	auto flush = [&]()
	{
		hexdump(start, &bytes[0], bytes.size());
		start += bytes.size();
		bytes.resize(0);
	};

	while (count < length)
	{
		bytes += recvbyte();
		count++;
		if (bytes.size() == 16)
			flush();
	}
	flush();
}

static void exec_read(const char* filename, uint32_t start, uint32_t length)
{
	FILE* fp = fopen(filename, "wb");
	if (!fp)
		error("Could not open output file: %s", strerror(errno));

	printf("Reading '%s' from RAM at address 0x%08X:\n", filename, start);

	Bytes stub(_obj_stubs_read_bin, _obj_stubs_read_bin_len);
	writebe32(&stub[2], start);
	writebe32(&stub[8], length);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
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
		uint8_t b = recvbyte();
		fputc(b, fp);

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

void cmd_read(char** argv)
{
	const char* filename = argv[0];
	const char* start = filename ? argv[1] : NULL;
	const char* length = start ? argv[2] : NULL;

	if (!filename || !start || !length || argv[3])
		error("syntax error: read <filename> <start> <length>");

	int64_t s = strtoll(start, NULL, 0);
	if ((s < 0) || (s > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	int64_t e = strtoll(length, NULL, 0);
	if ((e < 0) || (e > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	exec_read(filename, s, e);
};

void cmd_dump(char** argv)
{
	const char* start = argv[0];
	const char* length = start ? argv[1] : NULL;

	if (!start || !length || argv[2])
		error("syntax error: dump <start> <length>");

	int64_t s = strtoll(start, NULL, 0);
	if ((s < 0) || (s > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	int64_t e = strtoll(length, NULL, 0);
	if ((e < 0) || (e > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	exec_dump(s, e);
};


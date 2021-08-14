#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include <algorithm>

#include ".obj/stubs/fill_stub.h"

static void exec_fill(uint32_t start, uint32_t length, uint8_t value)
{
	Bytes stub(_obj_stubs_fill_bin, _obj_stubs_fill_bin_len);
	writebe32(&stub[2], start);
	writebe32(&stub[8], length);
	writebe16(&stub[14], value);
	pad_with_nops(stub);

	brecord_write(0xffffffc0, stub.size(), &stub[0]);
	brecord_execute(0xffffffc0);
};

void cmd_fill(char** argv)
{
	const char* start = argv[0];
	const char* length = start ? argv[1] : NULL;
	const char* value = length ? argv[2] : NULL;

	if (!start || !length || !value || argv[3])
		error("syntax error: fill <start> <length> value>");

	int64_t s = strtoll(start, NULL, 0);
	int64_t l = strtoll(length, NULL, 0);
	int64_t v = strtoll(value, NULL, 0);
	if ((s < 0) || (s > 0xFFFFFFFF) || (l < 0) || (l > 0xFFFFFFFF))
		error("syntax error: address range out of bounds");

	exec_fill(s, l, v);
};



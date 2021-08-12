#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include <algorithm>

using std::min;

/* --- Write RAM --------------------------------------------------------- */

static void exec_write(const char* filename, uint32_t start)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		error("Could not open output file: %s", strerror(errno));

	fseek(fp, 0, SEEK_END);
	uint32_t length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("Writing '%s' to RAM at address 0x%08X:\n", filename, start);

	resettimer();
	error("unsupported");
#if 0
	Packet p;
	p.request = PACKET_WRITE;

	uint32_t count = 0;
	for (;;)
	{
		int i = fread(p.data+6, 1, MaximumPacketSize-8, fp);
		if (i == 0)
			break;

		p.length = 6 + i;
		p.setq(0, count+start);
		p.sets(4, i);
		p.write();
		p.read();
		p.checkresponse(0x0085);

		count += i;

		printf("\r%03d%% complete: %d bytes (%d Bps)",
				(100*count) / length,
				count, (count*1000) / gettime());
		fflush(stdout);
	}
	printf("\r100\n");
#endif

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


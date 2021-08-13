#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include "globals.h"

static struct timeval timezero;

void warning(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	fprintf(stderr, "dbztool: ");
	vfprintf(stderr, message, ap);
	fprintf(stderr, "\n");

	va_end(ap);
};

void error(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	fprintf(stderr, "dbztool: ");
	vfprintf(stderr, message, ap);
	fprintf(stderr, "\n");

	va_end(ap);
	exit(1);
};

void verbose(const char* message, ...)
{
	if (Verbose)
	{
		va_list ap;
		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);
	}
};

std::string aprintf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int bytes = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	std::string s;
	s.resize(bytes);
	va_start(ap, fmt);
	vsnprintf(&s[0], bytes+1, fmt, ap);
	va_end(ap);

	return s;
}

void resettimer()
{
	gettimeofday(&timezero, NULL);
};

double gettime()
{
	struct timeval timenow;
	gettimeofday(&timenow, NULL);

	return (timenow.tv_sec - timezero.tv_sec) +
	       ((timenow.tv_usec - timezero.tv_usec) / 1e6);
};

void pad_with_nops(Bytes& stub)
{
	if (stub.size() > 32)
		error("stub is too large");

	while (stub.size() < 32)
	{
		stub += (char)0x4e;
		stub += (char)0x71;
	}
}

void writebe32(uint8_t* dest, uint32_t value)
{
	dest[0] = value>>24;
	dest[1] = value>>16;
	dest[2] = value>>8;
	dest[3] = value;
}

uint16_t readbe16(const uint8_t* ptr)
{
	return (ptr[0] << 8) | ptr[1];
}


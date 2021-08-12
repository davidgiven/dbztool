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

uint32_t gettime()
{
	struct timeval timenow;
	gettimeofday(&timenow, NULL);

	return ((timenow.tv_sec - timezero.tv_sec) * 1000) +
	       ((timenow.tv_usec - timezero.tv_usec) / 1000);
};


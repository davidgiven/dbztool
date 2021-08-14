#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"
#include <map>

static const std::map<std::string, Register> registers =
{
	{ "CSA",     { CSA, 16 }},
	{ "CSB",     { CSB, 16 }},
	{ "CSC",     { CSC, 16 }},
	{ "CSD",     { CSD, 16 }},
	{ "CSGBA",   { CSGBA, 16 }},
	{ "CSGBB",   { CSGBB, 16 }},
	{ "CSGBC",   { CSGBC, 16 }},
	{ "CSGBD",   { CSGBD, 16 }},
	{ "CSUGBA",  { CSUGBA, 16 }},
	{ "CEMUCS",  { EMUCS, 16 }},
	{ "CSCTRL1", { CSCTRL1, 16 }},
	{ "CSCTRL2", { CSCTRL2, 16 }},
};

const Register* find_register(const char* name)
{
	const auto& i = registers.find(name);
	if (i == registers.end())
		return nullptr;

	return &i->second;
}


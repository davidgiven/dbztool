#include <stdint.h>
#include <stdbool.h>

#define TX1H (*((volatile uint8_t*) 0xfffff906))
#define TX1L (*((volatile uint8_t*) 0xfffff907))

void kputc(char c)
{
	while (!(TX1H & (1<<5)))
		;
	TX1L = c;
}

void kputs(const char* s)
{
	for (;;)
	{
		char c = *s++;
		if (!s)
			return;
		kputc(c);
	}
}

void crash(const char* s)
{
	for (;;)
		kputs(s);
}

void bus_error()     { crash("bus error"); }
void address_error() { crash("address error"); }


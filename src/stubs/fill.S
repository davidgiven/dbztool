.globl _start

RX = 0xffffff904

_start:
	lea 0x12345678, %a2   | placeholder value, replaced
	movl #0x12345678, %d7 | placeholder value, replaced
loop:
	movb #0x88, (%a2)+    | placeholder value, replaced
	subql #1, %d7
	bneb loop
exit:


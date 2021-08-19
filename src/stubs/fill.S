.globl _start

RX = 0xffffff904
TX = 0xffffff906
EXIT = 0xffffff5a

| This is not placed in the instruction buffer, but instead somewhere in low
| memory. Use PIC only.

_start:
	lea 0x12345678, %a2   | placeholder value, replaced
	movl #0x12345678, %d7 | placeholder value, replaced
loop:
	movb #0x88, (%a2)+    | placeholder value, replaced
	subql #1, %d7
	bneb loop
exit:
	movb #0x40, TX+1
	jmp EXIT


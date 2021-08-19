.globl _start

RX = 0xfffff904
TX = 0xfffff906
EXIT = 0xffffff5a

| This is not placed in the instruction buffer, but instead somewhere in low
| memory. Use PIC only.

_start:
	movel #0x12345678, %a2
	movel #0x12345678, %d7
loop:
	movew RX, %d1
	btstl #13, %d1
	beqb loop
	moveb %d1, (%a2)+
	subql #1, %d7
	bneb loop
exit:
	jmp EXIT


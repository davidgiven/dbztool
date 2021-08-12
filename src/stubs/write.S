.globl _start

RX = 0xffffff904

_start:
	movel #0x12345678, %a2
	movel #0x12345678, %d7
loop:
	beqb exit
wait:
	movew RX, %d1
	btstl #13, %d1
	beqb wait
	moveb %d1, (%a2)+
	subql #1, %d7
	brab loop
exit:


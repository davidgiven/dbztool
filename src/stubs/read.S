.globl _start

TX = 0xffffff906

_start:
	movel #0x12345678, %a2
	movel #0x12345678, %d7
loop:
	beqb exit
wait:
	movew TX, %d1
	btstl #13, %d1
	beqb wait
	moveb (%a2)+, TX+1
	subql #1, %d7
	brab loop
exit:

.globl _start

RX = 0xffffff904
TX = 0xffffff906

.globl _start
_start:

RX = 0xffffff904

_start:
loop:
	movq #2-1, %d7
wait:
	movw RX, %d1
	btstl #13, %d1
	beqb wait
	lsll #8, %d0
	movb %d1, %d0
	dbf %d7, wait

	tstw %d0
	beqb exit
	movw %d0, %a2
	movb (%a2), TX+1
	brab loop
exit:

.globl _start

RX = 0xffffff904

_start:
loop:
	movq #3-1, %d7
	clr %d0
wait:
	movw RX, %d1
	btstl #13, %d1
	beqb wait
	lsll #8, %d0
	movb %d1, %d0
	dbf %d7, wait

	moveaw %d0, %a2
	swap %d0		 | move data from high word to low word
	beqb exit
	movb %d0, (%a2)
	brab loop
exit:

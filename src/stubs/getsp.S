.globl _start

TX = 0xffffff906

_start:
	movl %sp, %d7
	movql #2-1, %d6
outerloop:
	movql #4-1, %d1
loop:
	btstb #13-8, TX
	beqb loop
	roll #8, %d7
	moveb %d7, TX+1
	dbf %d1, loop

	movl %usp, %a0
	movl %a0, %d7
	dbf %d6, outerloop
exit:


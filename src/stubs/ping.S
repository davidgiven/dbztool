
TX = 0xffffff906

.globl _start
_start:
	movew TX, %d1
	btst #13, %d1
	beqb _start

	moveb #'P', TX+1


UBAUD1 = 0xfffff902

TX = 0xffffff906

.globl _start
_start:
	movw TX, %d1
	btst #15, %d1
	beqb _start

	//movw #0x0026, UBAUD1 // 38400 baud


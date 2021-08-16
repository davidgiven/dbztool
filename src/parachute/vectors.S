.globl _start
_start:
	.long 0				| initial SP
	.long 0				| initial PC
	.long bus_error     | bus error
	.long address_error	| address error
	.long 0				| illegal instruction
	.long 0				| divide-by-zero
	.long 0				| CHK instruction
	.long 0				| TRAPV instruction
	.long 0				| privilege violation
	.long 0				| trace
	.long 0				| line 1010 emulator
	.long 0				| line 1111 emulator
	.space 4*3   		| unused
	.long 0				| uninitialised interrupt
	.space 4*8          | unused
	.long 0				| spurious interrupt
	.long 0				| level 1 autovector
	.long 0				| level 2 autovector
	.long 0				| level 3 autovector
	.long 0				| level 4 autovector
	.long 0				| level 5 autovector
	.long 0				| level 6 autovector
	.long 0				| level 7 autovector
	.space 4*16			| TRAP
	.space 4*16			| unusued
	.space 4*192		| user interrupts


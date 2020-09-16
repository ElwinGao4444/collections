# assembly framework
#

.section .data
# datatype: (64bit is same as 32bit)
#	type		sizeof(byte)
#
#	.byte	'c'	1
#	.ascii	"str"	3
#	.asciz	"str"	3 + 1
#
#	.single	3.14	4
#	.float	3.14	4
#	.double	3.14	8
#
#	.short	0	2
#	.int	0	4
#	.long	0	4
#	.quad	0	8
#	.octa	0	16
#
	msg:
		.asciz "this is a test message: %d\n"
	factors:
		.double 37.45, 45.33, 12.30
	height:
		.int 54
	length:
		.int 62, 35, 47
	fill_buf:
		.fill 100	# notice the difference with .bss
.section .rodata
	.equ factor, 3
	.equ LINUX_SYS_CALL, 0x80

.section .bss
	.lcomm local_buf, 12	# can't declare in .global
	.comm global_buf, 12

.section .text
.global main

main:
# description of mov:
# x -> q:64bit, l:32bit, w:16bit, b:8bit
# movx source, destination
# rule of mov: P76
# indexed memory mode: base_address(offset_address, index, size) = base_address + offset_address + index * size
# indexed memory mode: offset_address & index must be register, size can be digit
#
	movl $factor, %eax
	movl $4, %eax

	pushl %eax
	pushl $msg
	call printf
	addl $8, %esp

# cmovx: P84
# cmp a, b means: set EFLAGS as the result of "b - a"
	movl $1, %eax
	movl $2, %ebx
	cmp %eax, %ebx		# can't compare whit digit
	cmova %ebx, %eax	# can't compare whit digit

	pushl %eax
	pushl $msg
	call printf
	addl $8, %esp

# exchange instruction P87
# xchg op1, op2 - (can't be memory both)
# bawap op - (little-endian <-> big-endian)
# xadd src, dst - (exchange and add) (src must be reg, dst can be a memory)
# cmpxchg src, dst - (CAS P89)
# cmpxchg8b (CAS 8byte)

# pushx source(x-q:64, x-l:32, x-w:16)
# popx destination
	pushl %eax	# 64bit don't allow pushl
	pushw %ax	# just pushq is legal
	popw %ax	# it's ok to push reg, mem, digit
	popl %eax	# it's ok to pop reg, mem
	pusha	# push  di, si, bp, bx, dx, cx, ax (pop reverse) (64bit invalid)
	pushal	# push edi,dsi,ebp,ebx,edx,ecx,eax (pop reverse) (64bit invalid)

# jmp location
# call address (ret)
# int irq
fun:
	pushl %ebp
	movl %esp, %ebp
	# do somgthine
	movl %ebp, %esp
	popl %ebp

# conditional jmp: P107 (can't cross section)
# cmp op1, op2 (op2 - op1)

# loop (sub 1 and cmp) (maybe ecx is 0 at first, jcxz can fix this problem)
# loope / loopz
# loopne / loopnz
	movl $100, %ecx
loop1:
	# do something
	loop loop1

	pushl $0
	call exit

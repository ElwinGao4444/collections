# loop.s - an example of the loop instrction

.section .data
	output: .asciz "the value is: %d\n"

.section .text
.global main

main:
	movl $100, %ecx		# try movl $0, %ecx (use jcxz to fix it)
	movl $0, %eax
loop1:
	addl %ecx, %eax
	loop loop1

	pushl %eax
	pushl $output
	call printf
	add $8, %esp

	movl $1, %eax
	movl $0, %ebx
	int $0x80

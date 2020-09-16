# find the max int - an example of the cmov instruction

.section .data
	output: .asciz "the largest value is %d\n"
	values: .int 105, 235, 61, 315, 134, 221, 53, 145, 117, 5

.section .text
	.global main

main:
	movl values, %ebx
	movl $1, %edi
loop:
	movl values(, %edi, 4), %eax
	cmp %ebx, %eax
	cmova %eax, %ebx
	inc %edi
	cmp $10, %edi
	jne loop

	pushl %ebx
	pushl $output
	call printf
	addl $8, %esp

	pushl $0
	call exit

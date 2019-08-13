#bubble.s - an example of xchg instruction

.section .data
val: 
	.int 105, 235, 61, 315, 134, 221, 53, 145, 117, 5

.section .text
.global main

main:
	movl $val, %esi
	movl $9, %ecx
	movl $9, %ebx
loop:
	movl (%esi), %eax
	cmp %eax, 4(%esi)
	jge skip
	xchg %eax, 4(%esi)
	movl %eax, (%esi)
skip:
	add $4, %esi
	dec %ebx
	jnz loop
	dec %ecx
	jz end

	movl $val, %esi
	movl %ecx, %ebx
	jmp loop
end:
	movl $1, %eax
	movl $0, %ebx
	int $0x80

#cpuid2.d view the cpuid vendor id string using c library calls

.section .data	# 数据段声明
	msg : .asciz "the processor vendor id is '%s'\n"	# 与ascii不同的是asciz会在最后添加‘\0’

.section .bss
	.lcomm buf, 12

.section .text	# 代码段声明
.global main	# 指定入口函数

main:		# 使用C库函数进行打印输出

	movl $0, %eax
	cpuid
	movl $buf, %edi
	movl %ebx, (%edi)
	movl %edx, 4(%edi)
	movl %ecx, 8(%edi)

	movl $buf, %esi	# 64位的库函数调用与32位时发生了很大的变化
	movl $msg, %edi # 不再使用压栈的方式进行传参，而是采用寄存器
	movl $0, %eax	# 直接传参的方式
	call printf

	movl $0, %eax
	call exit

# 如果直接用gcc编译，则不用考虑链接C库的问题
# 如果将main改为_start，采用as+ld的编译方式，则需要在ld时添加-lc参数链接C库

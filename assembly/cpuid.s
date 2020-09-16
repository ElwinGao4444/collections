#cpuid.d sample program to extract the processor vendor id

.section .data	# 数据段声明
	msg : .ascii "the processor vendor id is 'xxxxxxxxxxxx'\n"	# 要输出的字符串
	len = . - msg	# 字串长度

.section .text	# 代码段声明
.global _start	# 指定入口函数

_start:		# 在屏幕上显示一个字符串
	nop	# 解决GDB中"b *_start"无法正常断点的bug，此时使用"b *_start+1"即可
	
	movl $0, %eax
	cpuid
	movl $msg, %edi
	movl %ebx, 28(%edi)
	movl %edx, 32(%edi)
	movl %ecx, 36(%edi)
	
	movl $len, %edx	# 参数三：字符串长度
	movl $msg, %ecx	# 参数二：要显示的字符串
	movl $1, %ebx	# 参数一：文件描述符(stdout) 
	movl $4, %eax	# 系统调用号(sys_write) 
	int  $0x80	# 调用内核功能
	
	# 退出程序
	movl $0,%ebx	# 参数一：退出代码
	movl $1,%eax	# 系统调用号(sys_exit) 
	int  $0x80	# 调用内核功能

#hello.s sample program to print hello world information
.section .data    #数据段声明
msg:
    .ascii "hello world!\n"    #要输出的字符串
    len=.-msg                        #字符串长度

.section .text    #代码段声明
# .global main
# main:
.global _start     #指定入口函数

_start:                                 #函数在屏幕上输出hello world!
movl $len, %edx               #第三个参数： 字符串长度
movl $msg, %ecx             #第二个参数： hello world!字符串
movl $1, %ebx                  #第一个参数： 输出文件描述符
movl $4, %eax                  #系统调用号sys_write
int $0x80                            #调用内核功能

#下面为退出程序代码
movl $0, %ebx                #第一个参数： 退出返回码
movl $1, %eax                #系统调用sys_exit
int $0x80                    #调用内核功能

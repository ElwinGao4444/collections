package main

import (
	"fmt"
	"os"
	"time"
)

// 调试方法1:	// 类似go run的简单用法
// dlv debug debug.go	// 用dlv启动程序
// b main.main			// 对mian函数打断点
// b ./debug.go:n		// 在文件第n行打断点

// 调试方法2:	// 类似go build的常规用法
// go build debug.go && ./debug
// dlv attach PID	// 注意：go run启动的程序，或编译时带有-ldflags='-s -w'参数的程序，没有调试信息
// l main.main		// 可以先通过找到main函数，确定绝对路径
// b file:line		// 如果相对路径无法打点，则需要使用绝对路径

// 调试方法3:	// 使用GDB调试
// go build debug.go && ./debug
// gdb -p PID
// l main.main	// 即使使用gdb，也是使用和dlv一样的符号体系，但gdb中无法显示函数所在文件
// b file:line	// 如果相对路径无法打点，则需要使用绝对路径

func main() {
	fmt.Println("PID:", os.Getpid())
	for i := 1; i > 0; i++ {
		fmt.Print(i, "\r")
		time.Sleep(time.Duration(100) * time.Millisecond)
	}
}

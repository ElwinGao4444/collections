package main

import (
	"fmt"
	"time"
)

func main() {
	var n int
	var ok bool

	// ****************************************************************
	// channel的三种读取方法
	// ****************************************************************
	var c = make(chan int) // 缓冲区为0的channel，所有send都是阻塞式的，所以必须通过其他goroutine去send
	go func() { c <- 1 }()
	<-c // channel的第一种用法，只pop，不取值
	go func() { c <- 2 }()
	n = <-c // channel的第二种用法，pop后同时取值
	fmt.Println("n:", n)
	go func() { c <- 3 }()
	if n, ok = <-c; ok { // channel的第三种用法，pop取值时，同时检查channel的可读性
		fmt.Println("n:", n)
	}
	close(c)
	fmt.Println("n:", <-c)
	c = nil // 关闭channel后，channel仍然可读（返回0值），将channel置nil，会更加安全

	// ****************************************************************
	// 带缓存的channel
	// ****************************************************************
	c = make(chan int, 1) // 缓冲区>0时，channel未满时，不会阻塞，所以不需要通过其他goroutine去send
	c <- 0
	<-c
	close(c)

	// ****************************************************************
	// 使用for循环取channel
	// ****************************************************************
	c = make(chan int)
	go func() {
		for i := 0; i < 10; i++ {
			c <- i
		}
		close(c) // for range 方式访问channel，只有close才能使循环退出，如果不close会引起死锁
	}()
	for n := range c {
		fmt.Print(n)
	}
	fmt.Println("")

	// ****************************************************************
	// 使用for select读取channel
	// ****************************************************************
	c = make(chan int)
	go func() { c <- 1 }()
loop:
	for {
		select {
		case <-c:
			break loop // break只会跳出select，不会跳出for，所以需要通过label来处理
		}
	}
	close(c) // 关闭通道后，继续写会导致panic，继续读会导致立即返回“0”值，无论读写都会异常

	// ****************************************************************
	// 通过channel实现定时器
	// ****************************************************************
	select {
	case <-time.After(time.Duration(1) * time.Second):
		break
	}
}

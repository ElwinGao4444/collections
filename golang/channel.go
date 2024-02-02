package main

import (
	"fmt"
	"time"
)

func use_channel() {
	var c = make(chan int)
	go func() {
		time.Sleep(10 * time.Microsecond)
		c <- 1
	}()

loop:
	for {
		select {
		case n, ok := <-c:
			if !ok {
				c = nil
				continue
			}
			fmt.Println("chan recv", n)
		case <-time.After(1 * time.Second):
			fmt.Println("finish")
			break loop
		}
	}
	close(c) // 关闭通道后，继续写会导致panic，继续读会导致立即返回“0”值，无论读写都会异常
}

func bacis_usage() {
	var c = make(chan int) // 缓冲区为0的channel，所有send都是阻塞式的，所以必须放到其他goroutine中send
	var n int
	var ok bool
	go func() { c <- 1 }()
	<-c // channel的第一种用法，只pop，不取值
	go func() { c <- 2 }()
	n = <-c // channel的第二种用法，pop后同时取值
	fmt.Println("n:", n)
	go func() { c <- 3 }()
	if n, ok = <-c; ok { // channel的第三种用法，pop取值时，同时检查channel的可读性
		fmt.Println("n:", n)
	}
}

func main() {
	bacis_usage()
}

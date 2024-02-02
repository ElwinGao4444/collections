package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	fmt.Println("PID:", os.Getpid())
	for i := 1; i > 0; i++ {
		fmt.Print(i, "\r")
		time.Sleep(time.Duration(100) * time.Millisecond)
	}
}

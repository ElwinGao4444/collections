package main

import (
	"context"
	"fmt"
	"time"
)

func handle(ctx context.Context, duration time.Duration) {
	select {
	case <-ctx.Done(): //
		fmt.Println("handle", ctx.Err())
	case <-time.After(duration):
		fmt.Println("deadline", ctx.Deadline())
		fmt.Println("process request with", duration)
	}
}

// 基于超时的context
func use_timeout_context() {
	ctx, cancel := context.WithTimeout(context.Background(), 1*time.Second)
	defer cancel()

	go handle(ctx, 500*time.Millisecond)

	select {
	case <-ctx.Done():
		fmt.Println("main", ctx.Err())
	}
}

func main() {
	use_timeout_context()
}

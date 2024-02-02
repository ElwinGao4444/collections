package main

import (
	"context"
	"fmt"
	"time"
)

func handle(ctx context.Context, duration time.Duration) {
	select {
	case <-ctx.Done(): // 当context被cancel时，Done返回的chan会recv到数据
		fmt.Println("handle", ctx.Err())
	case <-time.After(duration):
		deadline, ok := ctx.Deadline() // 展示context的deadline时间
		fmt.Println("now", time.Now(), ok)
		fmt.Println("deadline", deadline, ok)
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

// 基于定时的context
func use_deadline_context() {
	ctx, cancel := context.WithDeadline(context.Background(), time.Now().Add(1*time.Second))
	defer cancel()

	go handle(ctx, 500*time.Millisecond)

	select {
	case <-ctx.Done():
		fmt.Println("main", ctx.Err())
	}
}

// 基于手动cancel的context
func use_cancel_context() {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	go handle(ctx, 500*time.Millisecond)

	time.Sleep(1 * time.Second)
	cancel()
	select {
	case <-ctx.Done():
		fmt.Println("main", ctx.Err())
	}
}

func main() {
	use_timeout_context()
	use_deadline_context()
	use_cancel_context()

	// context的基本读写方法
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	ctx1 := context.WithValue(ctx, "k1", "v1")  // context.WithValue会基于参数创建新context
	ctx2 := context.WithValue(ctx1, "k2", "v2") // 而并不会在原有context上进行追加
	ctx3 := context.WithValue(ctx2, "k3", "v3") // 这就是context确保多线程安全的手段

	fmt.Println("context:", ctx.Value("k1"))  // 原始ctx没有任何key
	fmt.Println("context:", ctx.Value("k2"))  // 原始ctx没有任何key
	fmt.Println("context:", ctx.Value("k3"))  // 原始ctx没有任何key
	fmt.Println("context:", ctx1.Value("k1")) // ctx1 只有k1
	fmt.Println("context:", ctx1.Value("k2"))
	fmt.Println("context:", ctx1.Value("k3"))
	fmt.Println("context:", ctx2.Value("k1")) // ctx2 只有k1
	fmt.Println("context:", ctx2.Value("k2")) // ctx2 只有k2
	fmt.Println("context:", ctx2.Value("k3"))
	fmt.Println("context:", ctx3.Value("k1")) // ctx3 全都有
	fmt.Println("context:", ctx3.Value("k2")) // ctx3 全都有
	fmt.Println("context:", ctx3.Value("k3")) // ctx3 全都有
}

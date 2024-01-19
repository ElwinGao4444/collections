package main

import (
	"fmt"
	"time"

	"github.com/valyala/fasthttp"
)

func handler(ctx *fasthttp.RequestCtx) {
	fmt.Fprintf(ctx, "hello")
}

func main() {
	server := &fasthttp.Server{
		Handler:      handler,
		ReadTimeout:  10 * time.Second,
		WriteTimeout: 10 * time.Second,
		Concurrency:  1024,
	}

	server.ListenAndServe(":8080")
}

package main

import (
	"fmt"
	"log"
	"time"

	"github.com/valyala/fasthttp"
)

func handler_wrap(ctx *fasthttp.RequestCtx) {
	fmt.Println("[Pre Request]")
	ctx.SetStatusCode(fasthttp.StatusNotFound)
	fmt.Println("[Post Request]")
}

func handler(ctx *fasthttp.RequestCtx) {
	fmt.Fprintf(ctx, "hello")
	ctx.SetStatusCode(fasthttp.StatusNotFound)
}

func run_simple_http_server2() {
	server := &fasthttp.Server{
		Handler:      handler,
		ReadTimeout:  10 * time.Second,
		WriteTimeout: 10 * time.Second,
		Concurrency:  1024,
	}

	go server.ListenAndServe(":8080")

}

func run_simple_http_server(addr string) {
	requestHandler := func(ctx *fasthttp.RequestCtx) {
		fmt.Fprintf(ctx, "Hello, world! Requested path is %q", ctx.Path())
	}

	if err := fasthttp.ListenAndServe(addr, requestHandler); err != nil {
		log.Fatalf("error in ListenAndServe: %v", err)
	}
}

func client_get(host string, port int, path string) {
	var url = fmt.Sprintf("http://%s:%d/%s", host, port, path)
	status, resp, err := fasthttp.Get(nil, url)
	fmt.Println("StatusCode: ", status)
	fmt.Println("Response: ", string(resp))
	fmt.Println("Error", err)
}

func main() {
	go run_simple_http_server(":8080")
	client_get("localhost", 8080, "/")
}

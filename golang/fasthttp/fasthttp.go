package main

import (
	"fmt"
	"log"
	"time"

	"github.com/valyala/fasthttp"
)

func new_standard_http_server() *fasthttp.Server {

	var handler = func(ctx *fasthttp.RequestCtx) {
		fmt.Fprintf(ctx, "hello")
	}

	// var handler_wrap = func(ctx *fasthttp.RequestCtx) {
	// 	fmt.Println("[Pre Request]")
	// 	ctx.SetStatusCode(fasthttp.StatusNotFound)
	// 	fmt.Println("[Post Request]")
	// }

	server := &fasthttp.Server{
		Handler:      handler,
		ReadTimeout:  10 * time.Second,
		WriteTimeout: 10 * time.Second,
		Concurrency:  1024,
	}

	return server
}

func start_standard_http_server(srv *fasthttp.Server, addr string) {
	if err := srv.ListenAndServe(addr); err != nil {
		log.Fatalf("error in ListenAndServe: %v", err)
	}
}

func stop_standard_http_server(srv *fasthttp.Server) {
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

	var srv = new_standard_http_server()
	go start_standard_http_server(srv, ":8081")
	client_get("localhost", 8081, "/")
	stop_standard_http_server(srv)
}

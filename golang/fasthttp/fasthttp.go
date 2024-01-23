package main

import (
	"fmt"
	"log"
	"time"

	"github.com/fasthttp/router"
	"github.com/valyala/fasthttp"
)

type cls struct{}

func (c cls) cls_handler(ctx *fasthttp.RequestCtx) {
	fmt.Fprintf(ctx, "Member Function: Requested path is %q", ctx.Path())
}

func new_standard_http_server() *fasthttp.Server {

	var handler = func(ctx *fasthttp.RequestCtx) {
		fmt.Fprintf(ctx, "Standard Server: Requested path is %q", ctx.Path())
	}

	router := router.New()
	router.GET("/", handler)
	router.GET("/cls", cls{}.cls_handler)
	// 此处可以自定义任何NotFound的行为
	router.NotFound = fasthttp.FSHandler("./", 0) // 这个例子是作为静态页展示，使用BodyStream缓存

	var h fasthttp.RequestHandler
	var r bool
	h, r = router.Lookup("GET", "/", nil)
	fmt.Println("lookup GET /: ", h, r)
	h, r = router.Lookup("GET", "/cls", nil)
	fmt.Println("lookup GET /cls: ", h, r)
	h, r = router.Lookup("GET", "//", nil)
	fmt.Println("lookup GET //: ", h, r)
	h, r = router.Lookup("GET", "/cls/", nil)
	fmt.Println("lookup GET /cls/: ", h, r)

	// 注意：fasthttp中有两个Body缓存，一个叫BodyBuffer，一个叫BodyStream
	// 这两个Body缓存是互斥的，使用一个的时候，会先强制清空另一个，从而导致“数据丢失”的现象
	var handler_wrap = func(ctx *fasthttp.RequestCtx) {
		fmt.Println("[Pre Request]")
		ctx.WriteString("[Pre Request]") // 使用BodyBuffer缓存
		router.Handler(ctx)
		fmt.Println("[Post Request]")
		ctx.WriteString("[Post Request]") // 使用BodyBuffer缓存

		ctx.SetStatusCode(fasthttp.StatusInternalServerError) // 强制覆盖StatusCode
	}

	server := &fasthttp.Server{
		Handler:      handler_wrap,
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
	srv.Shutdown()
}

func run_simple_http_server(addr string) {
	requestHandler := func(ctx *fasthttp.RequestCtx) {
		fmt.Fprintf(ctx, "Simple Server: Requested path is %q", ctx.Path())
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
	client_get("localhost", 8081, "/cls")
	client_get("localhost", 8081, "/index.html")
	stop_standard_http_server(srv)
}

package main

import (
	"context"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"strconv"
	"time"
)

type cls struct{}

func (c cls) cls_handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Member Function: Requested path is %s", r.URL.Path)
}

// 最简单的启动方法，使用默认路由，且无法主动退出
func start_simple_http_server(port int) error {
	// 此处的Multiplexer隐式注册在全局的defaultServeMux中（可以通过http.DefaultServeMux访问到）
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "Simple Server: Requested path is %s", r.URL.Path)
	})
	http.HandleFunc("/cls", cls{}.cls_handler)
	fmt.Println("resp: ", http.DefaultServeMux)

	var err error = nil
	// 此处的Server是在ListenAndServe方法中，临时创建的匿名Server，无法被外部访问
	if err = http.ListenAndServe(":"+strconv.Itoa(port), nil); err != nil && err != http.ErrServerClosed {
		fmt.Printf("http server failed, err:%v\n", err)
	}
	return err
}

func new_standard_http_server(port int) *http.Server {
	var handler = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "Standard Server: Requested path is %s", r.URL.Path)
	})

	// 注意：ServeMux的处理流程，并不是完全严格匹配，而是当匹配失败时，会使用最长前缀匹配
	//       所以，根目录"/"会兜底一切未注册路由，这种做法虽然提升了容错性，但极不安全
	var mymux = http.NewServeMux()
	mymux.HandleFunc("/", handler)
	mymux.HandleFunc("/cls", cls{}.cls_handler)

	var handler_wrap = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// 注意：ResponseWriter的Write方法，会自动对空StatusCode赋值为http.StatusOK，非空则不覆盖
		//       详见http.ResponseWriter接口中Write方法的注释
		fmt.Println("[Pre Request]")
		// w.Write([]byte("[Pre Request]"))	// 首次写入会StatusCode 200，导致mymux.ServeHTTP无法正确写入StatusCode
		mymux.ServeHTTP(w, r)
		fmt.Println("[Post Request]")
		w.Write([]byte("[Post Request]")) // mymux.ServeHTTP已经写入了StatusCode，此处追加信息，不会覆盖StatusCode
	})

	var mysrv = &http.Server{
		Addr:    ":" + strconv.Itoa(port),
		Handler: handler_wrap,
	}

	return mysrv
}

func start_standard_http_server(srv *http.Server) error {
	var err error = nil
	if err = srv.ListenAndServe(); err != nil && err != http.ErrServerClosed {
		log.Fatalf("listen: %s\n", err)
	}
	return err
}

func stop_standard_http_server(srv *http.Server) error {
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer func() {
		// extra handling here
		cancel()
	}()

	var err error = nil
	if err := srv.Shutdown(ctx); err != nil {
		log.Fatalf("Server Shutdown Failed:%+v", err)
	}

	return err
}

func client_get(host string, port int, path string) {
	var url = fmt.Sprintf("http://%s:%d/%s", host, port, path)
	if resp, err := http.Get(url); err == nil {
		if body, err := ioutil.ReadAll(resp.Body); err == nil {
			fmt.Println("StatusCode: ", resp.StatusCode)
			fmt.Println("Response: ", string(body))
			fmt.Println("Error", err)
		}
	}
}

func main() {
	// Simple Server用法
	go start_simple_http_server(8080)
	client_get("localhost", 8080, "/")
	client_get("localhost", 8080, "/cls")

	// Standard Server用法
	var srv = new_standard_http_server(8081)
	go start_standard_http_server(srv)
	client_get("localhost", 8081, "/")
	client_get("localhost", 8081, "/cls")
	stop_standard_http_server(srv)
}

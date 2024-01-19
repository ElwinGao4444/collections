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

// 最简单的启动方法，使用默认路由，且无法主动退出
func start_simple_http_server(port int) error {
	// 此处的Multiplexer隐式注册在全局的defaultServeMux中（可以通过http.DefaultServeMux访问到）
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "Simple Server")
	})
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
		fmt.Fprint(w, "Standard Server")
	})

	var mymux = http.NewServeMux()
	mymux.HandleFunc("/", handler)

	var handler_wrap = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("[Pre Request]")
		mymux.ServeHTTP(w, r)
		fmt.Println("[Post Request]")
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
			fmt.Println("resp: ", string(body))
		}
	}
}

func main() {
	// // Simple Server用法
	// go start_simple_http_server(8080)
	// client_get("localhost", 8080, "/")

	// Standard Server用法
	var srv = new_standard_http_server(8081)
	go start_standard_http_server(srv)
	client_get("localhost", 8081, "/")
	stop_standard_http_server(srv)
}

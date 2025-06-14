package main

import (
	"fmt"
	"html"
	"log"
	"log/slog"
	"net/http"
	"os"
	"time"
)

func main() {
	// 配置本地日志文件
	logFile, err := os.OpenFile("file.log", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatal(err)
	}
	defer logFile.Close()
	log.SetOutput(logFile) // log模块输出到文件
	logger := slog.New(slog.NewJSONHandler(os.Stdout, nil))

	// 启动http服务
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		log.Printf("Log generated at: %v\n", time.Now().Format("2006-01-02 15:04:05"))
		logger.Info("Log generated", "time", time.Now().Format("2006-01-02 15:04:05"))
		fmt.Fprintf(w, "gateway recv: %q", html.EscapeString(r.URL.Path))
	})
	log.Fatal(http.ListenAndServe(":8080", nil))
}

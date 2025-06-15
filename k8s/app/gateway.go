package main

import (
	"context"
	"fmt"
	"html"
	"log"
	"log/slog"
	"net/http"
	"os"
	"time"

	"github.com/go-redis/redis/v8"
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

	// 创建Redis客户端
	client := redis.NewClient(&redis.Options{
		Addr:     os.Getenv("redis_host") + ":" + os.Getenv("redis_port"), // Redis服务器地址
		Password: os.Getenv("redis_pass"),                                 // 密码，如果没有密码则为空字符串
		DB:       0,                                                       // 使用的数据库编号
	})
	// Ping测试连接
	pong, err := client.Ping(context.Background()).Result()
	fmt.Println(pong, err)

	// 注册路由
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		log.Printf("Log generated at: %v\n", time.Now().Format("2006-01-02 15:04:05"))
		logger.Info("Log generated", "time", time.Now().Format("2006-01-02 15:04:05"))
		fmt.Fprintf(w, "gateway recv: %q", html.EscapeString(r.URL.Path))
	})
	http.HandleFunc("/redis", func(w http.ResponseWriter, r *http.Request) {
		var key = r.URL.Query().Get("key")
		logger.Info("query", "key", key)
		val, err := client.Get(context.Background(), key).Result()
		logger.Info("query", "result", val)
		if err != nil {
			fmt.Fprintf(w, "redis get err: %q", err)
			return
		}
		fmt.Fprintf(w, "redis get key: %q, value: %q", key, val)
	})

	// 启动http服务
	log.Fatal(http.ListenAndServe(":8080", nil))
}

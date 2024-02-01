package main

// 感谢Blog：https://www.51cto.com/article/722242.html

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"log/slog"
	"os"
)

// 原生log库，功能非常弱，通常只能用于极小的项目，或是调试性的使用
func use_log() {
	// Print系列
	log.Print("log.Print: ", "适用于连续字符型输出")
	log.Printf("log.Printf: %s", "适用于自定义格式输出")
	log.Println("log.Println:", "适用于懒人输出（自动空格分隔）")

	// Fatal系列（与Print系列用法相同，驶入输出后会自动调用os.Exit(1)退出程序）
	// log.Fatal("log.Fatal: ", "log.Print and os.Exit(1)")
	// log.Fatalf("log.Fatalf: %s", "log.Printf and os.Exit(1)")
	// log.Fatalln("log.Fatalln:", "log.Println and os.Exit(1)")

	// Panic系列（将Print最终输出的字符串作为panic的参数，并执行panic(...)方法）
	// log.Panic("log.Panic: ", "panic(log_str)")
	// log.Panicf("log.Panicf: %s", "panic(log_str)")
	// log.Panicln("log.Panicln:", "panic(log_str)")

	// 自定义前缀
	log.SetPrefix("[DIY Prefix]")
	log.Println("在默认日志格式前，增加自定义前缀")
	log.SetPrefix("")

	// 通过Flag配置基本日志格式
	log.SetFlags(log.LstdFlags)
	log.Println("通过Flag配置基本日志格式, 具体参数枚举可直接查看源码")

	// 自定义Logger（可定制输出到不同的输出端）
	var logger *log.Logger
	buf := &bytes.Buffer{}
	logger = log.New(buf /*output*/, "" /*prefix*/, log.LstdFlags /*flags*/)
	logger.Println("log in buffer")
	fmt.Print(buf.String())
	// 多端输出
	writer1 := &bytes.Buffer{}
	writer2 := os.Stdout
	writer3, _ := os.OpenFile("tmp.log", os.O_WRONLY|os.O_CREATE, 0640)
	logger = log.New(io.MultiWriter(writer1, writer2, writer3), "", log.Lshortfile|log.LstdFlags)
	logger.Println("log in multi-output")
}

// 1.21最新放到标准库的slog，相比原生log更强大，但偏向于Json格式化日志，并不提供format形式的日志能力
func use_slog() {
	// slog的顺序KV日志（slog没有像log一样，有完全灵活的输出能力，而是用k-v对的结构化形式）
	slog.Debug("slog.Debug message", "k1", "v1", "k2", 2, "k3", 3.0)
	slog.Info("slog.Info message", "k1", "v1", "k2", 2, "k3", 3.0)
	slog.Warn("slog.Warn message", "k1", "v1", "k2", 2, "k3", 3.0)
	slog.Error("slog.Error message", "k1", "v1", "k2", 2, "k3", 3.0)

	// slog结构化日志
	slog.Info("slog.Info message",
		slog.Int("int", 1),
		slog.Float64("float", 2.0),
		slog.String("string", "v-str"),
		slog.Group("Group message",
			slog.Bool("bool", true),
			slog.Any("any", use_slog),
		),
	)

	// slog的输出格式
	text_logger := slog.New(slog.NewTextHandler(os.Stderr, nil)) // text格式输出
	text_logger.Info("message", "k", "v")
	text_logger.Info("message", "map", map[string]string{"k": "v"})
	json_logger := slog.New(slog.NewJSONHandler(os.Stderr, nil)) // json格式输出
	json_logger.Info("message", "k", "v")
	json_logger.Info("message", "map", map[string]string{"k": "v"})

	var logger *slog.Logger = nil

	// 通过Option指定输出Source调试信息并调整日志输出级别
	logger = slog.New(slog.NewJSONHandler(os.Stderr, &slog.HandlerOptions{AddSource: true, Level: slog.LevelDebug}))
	logger.Debug("msg", "k", "v") // 默认情况下，Debug日志是不输出的

	// 增加日志的自定义属性
	logger = slog.Default().With("attr-key", "attr-value")
	logger.Info("msg", "k1", "v1")
	logger.Info("msg", "k2", 2)
	logger.Info("msg", "k3", 3.0)

	// 增加日志分组
	logger = json_logger.WithGroup("group_name")
	logger.Info("msg", "k1", "v1")
	logger.Info("msg", "k2", 2)
	logger.Info("msg", "k3", 3.0)

	// 动态调整日志级别
	var lv_var slog.LevelVar
	lv_var.Set(slog.LevelInfo)
	slog.SetDefault(slog.New(slog.NewTextHandler(os.Stderr, &slog.HandlerOptions{Level: &lv_var})))
	slog.Debug("slog.Debug", "k", "v")
	lv_var.Set(slog.LevelDebug)
	slog.Debug("slog.Debug", "k", "v")
}

func main() {
	use_log()  // 参考自：https://darjun.github.io/2020/02/07/godailylib/log/
	use_slog() // 参考自：https://tonybai.com/2023/09/01/slog-a-new-choice-for-logging-in-go/
}

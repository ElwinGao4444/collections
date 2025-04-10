package main

import (
	"html/template"
	"os"
)

type VarStruct struct {
	VarStr          string
	VarInt          int
	VarSliceInt     []int
	VarMapStringInt map[string]int
	VarChanInt      chan int
	VarFunc         func(int) int // 函数既可以通过变量形式注入，引入可以通过FuncMap形式注入
}

func main() {
	var tmpl *template.Template

	var vs = VarStruct{
		VarStr:          "zhangsan",
		VarInt:          3,
		VarSliceInt:     []int{1, 2, 3},
		VarMapStringInt: map[string]int{"a": 1, "b": 2, "c": 3},
		VarChanInt:      make(chan int),
		VarFunc: func(n int) int {
			return n + 100
		},
	}

	go func() {
		vs.VarChanInt <- 11
		vs.VarChanInt <- 12
		vs.VarChanInt <- 13
		close(vs.VarChanInt) // 不close会导致死锁
	}()

	var FuncsMap = template.FuncMap{
		"inc": func(n int) int { // 单返回值
			return n + 1
		},
		"asc": func(n int) (string, error) { // 双返回值
			return string(n), nil
		},
	}

	tmpl, _ = template.New("demo").Parse("0. 顶级作用域下的整体替换：{{.}}\n")
	tmpl.Execute(os.Stdout, "hello world")

	tmpl, _ = template.New("demo.tmpl").Funcs(FuncsMap).ParseFiles("demo.tmpl") // 对于后续使用ParseFiles的情况，New的参数必须是文件名
	tmpl.Execute(os.Stdout, vs)
}

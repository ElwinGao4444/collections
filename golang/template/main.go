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
	// VarChanInt      chan int
}

func main() {
	var tmpl *template.Template

	var vs = VarStruct{
		VarStr:          "zhangsan",
		VarInt:          3,
		VarSliceInt:     []int{1, 2, 3},
		VarMapStringInt: map[string]int{"a": 1, "b": 2, "c": 3},
	}

	tmpl, _ = template.New("demo").Parse("0. 顶级作用域下的整体替换：{{.}}\n")
	tmpl.Execute(os.Stdout, "hello world")

	tmpl, _ = template.ParseFiles("demo.tmpl")
	tmpl.Execute(os.Stdout, vs)
}

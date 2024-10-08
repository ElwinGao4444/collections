package main

import (
	"fmt"
	"reflect"
)

type A struct {
	i int
	b *B
}

type B struct {
	s string
}

func print_all_element(i interface{}) {
	var e = reflect.ValueOf(i).Elem() // Elem()获取指针指向的值
	for n := 0; n < e.NumField(); n++ {
		var f = e.Field(n)
		var t = f.Type()
		if t.Kind() == reflect.Ptr {
			fmt.Println("pointer:", n, f, t)
		} else {
			fmt.Println("buildin:", n, f, t)
		}
	}
}

func main() {
	var a = A{i: 1, b: &B{s: "a"}}
	var t = reflect.TypeOf(a)
	var v = reflect.ValueOf(a)
	fmt.Println("通过获取类型信息：", t)
	fmt.Println("通过获取值信息：", v)
	print_all_element(&a) // 只有指针类型才能获取Elem()
}

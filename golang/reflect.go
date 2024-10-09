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
	c C
}

type C struct {
	f float32
}

// 返回指定变量的反射信息
func tv(i interface{}) (reflect.Type, reflect.Value) {
	return reflect.TypeOf(i), reflect.ValueOf(i)
}

// 对反射信息进行解引用（如果是指针类型的话）
func deref(t reflect.Type, v reflect.Value) (reflect.Type, reflect.Value) {
	if t.Kind() == reflect.Ptr {
		t = t.Elem()
		v = v.Elem()
	}
	return t, v
}

// 对反射信息进行输出
func show(n string, t reflect.Type, v reflect.Value) reflect.Kind {
	var k = t.Kind()
	t, v = deref(t, v)
	fmt.Println(k, t.Name(), n, v)
	return k
}

// 打印指定数据类型的反射信息
func print_all_element(n string, i interface{}) {
	t, v := tv(i)
	print_all_element_recursive(n, t, v)
}

// 对指定的反射信息进行递归遍历
func print_all_element_recursive(name string, fieldType reflect.Type, fieldValue reflect.Value) {
	show(name, fieldType, fieldValue)
	fieldType, fieldValue = deref(fieldType, fieldValue)
	for i := 0; i < fieldType.NumField(); i++ {
		var n = fieldType.Field(i).Name
		var t = fieldType.Field(i).Type
		var v = fieldValue.Field(i)
		if t.Kind() == reflect.Ptr && v.IsNil() {
			v = reflect.New(t.Elem())
		}
		if t.Kind() == reflect.Struct || t.Kind() == reflect.Ptr {
			print_all_element_recursive(n, t, v)
		}
	}
}

func main() {
	var a = A{i: 1, b: &B{s: "a"}}
	var t = reflect.TypeOf(a)
	var v = reflect.ValueOf(a)
	fmt.Println("通过获取类型信息：", t)
	fmt.Println("通过获取值信息：", v)
	fmt.Println("----------------")
	fmt.Println("输出结构信息：")
	print_all_element("a", a)
}

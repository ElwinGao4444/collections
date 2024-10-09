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
func show(t reflect.Type, v reflect.Value) reflect.Kind {
	var k = t.Kind()
	if k == reflect.Ptr {
		fmt.Println(k, t.Elem().Name(), v.Elem()) // Elem()无论对Type和Value都有类似解引用的作用
	} else {
		fmt.Println(k, t.Name(), v)
	}
	return k
}

// 打印指定数据类型的反射信息（非递归）
func print_all_element(i interface{}) {
	fieldType, fieldValue := tv(i)
	show(fieldType, fieldValue)
	fieldType, fieldValue = deref(fieldType, fieldValue)
	for n := 0; n < fieldType.NumField(); n++ {
		var t = fieldType.Field(n).Type
		var v = fieldValue.Field(n)
		show(t, v)
	}
}

// 对指定的反射信息进行递归遍历
func print_all_element_recursive(fieldType reflect.Type, fieldValue reflect.Value) {
	show(fieldType, fieldValue)
	fieldType, fieldValue = deref(fieldType, fieldValue)
	for n := 0; n < fieldType.NumField(); n++ {
		var t = fieldType.Field(n).Type
		var v = fieldValue.Field(n)
		if t.Kind() == reflect.Ptr && v.IsNil() {
			v = reflect.New(t.Elem())
		}
		if t.Kind() == reflect.Struct || t.Kind() == reflect.Ptr {
			print_all_element_recursive(t, v)
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
	print_all_element(a)
	fmt.Println("----------------")
	fmt.Println("输出结构信息（递归）：")
	print_all_element_recursive(tv(a))
}

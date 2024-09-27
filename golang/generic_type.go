package main

import (
	"fmt"
)

// 总体来说，Golang的泛型限制很多，并不好用，只能在一些特殊情况下才有使用场景

// 定义泛型函数
// 注意：Golang只支持了泛型，但并没有支持运算符重载，所以在使用基础运算符运算时，是无法支持类类型的
func my_func[T int | float64 | string](a, b T) T {
	return a + b
}

// 定义泛型对象
type my_struct[T int | float64 | string, S []T] struct {
	title   string
	content T // 直接泛型
	history S // 泛型嵌套
}

// 定义泛型方法
// 注意：对象方法只能用对象已经定义的泛型类型，不能定义对象中不存在的新泛型类型
func (st my_struct[T, S]) getHistory() S {
	return st.history
}

func main() {
	// 定义泛型切片
	type my_slice[T int | float64 | string] []T
	var s my_slice[int] = []int{1, 2, 3}
	fmt.Println("slice int:", s)

	// 定义泛型Map
	type my_map[key int | string, value string | float64] map[key]value
	var m my_map[string, float64] = map[string]float64{"a": 1.1, "b": 2.2}
	fmt.Println("map[string]float64 :", m)

	// 使用泛型函数
	fmt.Println("sum int:", my_func(1, 2))      // 隐式推导泛型类型
	fmt.Println("sum int:", my_func[int](1, 2)) // 显式指定泛型类型
	fmt.Println("sum float:", my_func(1.1, 2.2))
	fmt.Println("sum string:", my_func("a", "b"))

	// 使用泛型对象
	var st my_struct[int, []int]
	st.title = "ttt"
	st.content = 123
	st.history = []int{123, 234, 345, 456, 567}
	fmt.Println("struct:", st)
	fmt.Println("struct history:", st.getHistory())
}

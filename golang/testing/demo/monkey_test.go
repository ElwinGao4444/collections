package demo

import (
	reflect "reflect"
	"testing"

	"bou.ke/monkey"
)

// 第三方GoMonkey用法（在MaxOS m1芯片上，需要增加环境变量：GOARCH=amd64）
// 一般需要在编译参数上，增加"-gcflags=-l"，禁止内联优化

func TestMonkey(t *testing.T) {
	// 对指定函数进行Mock
	monkey.Patch(bar, func(n int) int {
		return n + 2
	})

	if n := foo(1); n != 3 {
		t.Errorf("result = %v", n)
	}

	// 对成员方法进行Mock
	var testUser = &User{}
	monkey.PatchInstanceMethod(reflect.TypeOf(testUser), "Handle", func(_ *User, n int) int {
		return n + 1
	})
	if n := testUser.Handle(0); n != 1 {
		t.Errorf("result = %v", n)
	}
}

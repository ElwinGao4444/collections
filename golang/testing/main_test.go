package main

import (
	"testing"
)

/****************************************************************/
// Testing
/****************************************************************/
// 原生Testing用法
// go test -v -run TestHelloWorld main_test.go
func TestHelloWorld(t *testing.T) {
	t.Log("ok")
}

package demo

import (
	"testing"

	"github.com/smartystreets/goconvey/convey"
	"github.com/stretchr/testify/assert"
)

// 原生Testing用法
// go test -v -run TestHelloWorld main_test.go
func TestHelloWorld(t *testing.T) {
	t.Log("ok")
	t.Error("error") // 报错但不退出
	t.Fatal("fatal") // 报错并且退出
}

// Assert库的用法
// go test -v -run TestAssert main_test.go
func TestAssert(t *testing.T) {
	assert.Equal(t, 123, 123, "they should be equal")
}

// Convey库的用法
// go test -v -run TestConvey main_test.go
func TestConvey(t *testing.T) {
	convey.Convey("TestConvey", t, func() {
		convey.So(123, convey.ShouldEqual, 123)
	})
}

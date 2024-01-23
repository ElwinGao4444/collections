package main

import (
	"testing"

	"github.com/smartystreets/goconvey/convey"
	"github.com/stretchr/testify/assert"
)

// go test -v -run TestHelloWorld main_test.go
func TestHelloWorld(t *testing.T) {
	t.Error("not implemented")
	t.Fatal("not implemented")
}

// go test -v -run TestAssert main_test.go
func TestAssert(t *testing.T) {
	assert.Equal(t, 123, 123, "they should be equal")
}

// go test -v -run TestConvey main_test.go
func TestConvey(t *testing.T) {
	convey.Convey("TestConvey", t, func() {
		convey.So(123, convey.ShouldEqual, 123)
	})
}

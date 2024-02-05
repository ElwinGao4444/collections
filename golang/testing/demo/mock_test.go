package demo

import (
	reflect "reflect"
	"testing"

	"bou.ke/monkey"
	gomock "github.com/golang/mock/gomock"
)

// ****************************************************************
// 原生GoMock用法
// ****************************************************************
//
// GoMock安装
// go get -u github.com/golang/mock/gomock
// go get -u github.com/golang/mock/mockgen
// go install github.com/golang/mock/mockgen
// mockgen -version
//
// GoMock文档
// go doc github.com/golang/mock/gomock

// 模式1：源文件模式，Mock指定文件中所有的接口
// mockgen -source=./demo.go -destination=./demo_mock.go -package=demo
// 模式2：接口模式，Mock指定路径下指定的接口
// mockgen -package=demo . Inter > ./demo.mock && mv ./demo.mock ./demo_mock.go
func TestGoMock(t *testing.T) {
	// mock控制器初始化
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	var mockInter *MockInter
	var testUser *User

	// 简单用法
	mockInter = NewMockInter(mockCtrl)
	mockInter.EXPECT().Foo(0).Return(1).Times(1)
	mockInter.EXPECT().Bar(0).Return(2).Times(1)
	testUser = &User{Inter: mockInter}
	if n := testUser.Use(0, 0); n != 3 {
		t.Errorf("result = %v", n)
	}

	// 通配用法
	mockInter = NewMockInter(mockCtrl)
	mockInter.EXPECT().Foo(gomock.Any()).Return(1).AnyTimes()
	mockInter.EXPECT().Bar(gomock.Any()).Return(2).AnyTimes()
	testUser = &User{Inter: mockInter}
	if n := testUser.Use(0, 0); n != 3 {
		t.Errorf("result = %v", n)
	}

	// 顺序用法(After)
	mockInter = NewMockInter(mockCtrl)
	var c1 = mockInter.EXPECT().Foo(gomock.Any()).Return(1).Times(1)
	var c2 = mockInter.EXPECT().Bar(gomock.Any()).Return(2).Times(1)
	mockInter.EXPECT().Foo(gomock.Any()).Return(3).Times(1).After(c1)
	mockInter.EXPECT().Bar(gomock.Any()).Return(4).Times(1).After(c2)
	testUser = &User{Inter: mockInter}
	if n := testUser.Use(0, 0); n != 3 {
		t.Errorf("result = %v", n)
	}
	if n := testUser.Use(0, 0); n != 7 {
		t.Errorf("result = %v", n)
	}

	// 顺序用法(InOrder)
	mockInter = NewMockInter(mockCtrl)
	gomock.InOrder(
		mockInter.EXPECT().Foo(gomock.Any()).Return(1).Times(1),
		mockInter.EXPECT().Bar(gomock.Any()).Return(2).Times(1),
		mockInter.EXPECT().Foo(gomock.Any()).Return(3).Times(1),
		mockInter.EXPECT().Bar(gomock.Any()).Return(4).Times(1),
	)
	testUser = &User{Inter: mockInter}
	if n := testUser.Use(0, 0); n != 3 {
		t.Errorf("result = %v", n)
	}
	if n := testUser.Use(0, 0); n != 7 {
		t.Errorf("result = %v", n)
	}

	// 自定义Mock的行为
	mockInter = NewMockInter(mockCtrl)
	mockInter.EXPECT().Foo(gomock.Any()).DoAndReturn(func(n int) int { return n + 10 })
	mockInter.EXPECT().Bar(gomock.Any()).DoAndReturn(func(n int) int { return n + 100 })
	testUser = &User{Inter: mockInter}
	if n := testUser.Use(0, 0); n != 110 {
		t.Errorf("result = %v", n)
	}
}

// ****************************************************************
// 第三方GoMonkey用法
// ****************************************************************
//
// GoMonkey安装
// go get bou.ke/monkey
//
// 在MaxOS m1芯片上，需要增加环境变量：GOARCH=amd64
// 一般需要在编译参数上，增加"-gcflags=-l"，禁止内联优化
func TestBouKeMonkey(t *testing.T) {
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

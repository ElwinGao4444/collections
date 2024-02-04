package demo

import (
	"testing"

	gomock "github.com/golang/mock/gomock"
)

// 原生GoMock用法
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
func TestGoMockSourceMode(t *testing.T) {
	// mock控制器初始化
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	// 简单用法
	mockInter1 := NewMockInter(mockCtrl)
	mockInter1.EXPECT().Foo(0).Return(1).Times(1)
	mockInter1.EXPECT().Bar(0).Return(2).Times(1)
	testUser1 := &User{Inter: mockInter1}
	if n := testUser1.Use(); n != 3 {
		t.Errorf("result = %v", n)
	}

	// 通配用法
	mockInter2 := NewMockInter(mockCtrl)
	mockInter2.EXPECT().Foo(gomock.Any()).Return(1).AnyTimes()
	mockInter2.EXPECT().Bar(gomock.Any()).Return(2).AnyTimes()
	testUser2 := &User{Inter: mockInter2}
	if n := testUser2.Use(); n != 3 {
		t.Errorf("result = %v", n)
	}

	// 顺序用法(After)
	mockInter3 := NewMockInter(mockCtrl)
	var c1 = mockInter3.EXPECT().Foo(gomock.Any()).Return(1).Times(1)
	var c2 = mockInter3.EXPECT().Bar(gomock.Any()).Return(2).Times(1)
	mockInter3.EXPECT().Foo(gomock.Any()).Return(3).Times(1).After(c1)
	mockInter3.EXPECT().Bar(gomock.Any()).Return(4).Times(1).After(c2)
	testUser3 := &User{Inter: mockInter3}
	if n := testUser3.Use(); n != 3 {
		t.Errorf("result = %v", n)
	}
	if n := testUser3.Use(); n != 7 {
		t.Errorf("result = %v", n)
	}

	// 顺序用法(InOrder)
	mockInter4 := NewMockInter(mockCtrl)
	gomock.InOrder(
		mockInter4.EXPECT().Foo(gomock.Any()).Return(1).Times(1),
		mockInter4.EXPECT().Bar(gomock.Any()).Return(2).Times(1),
		mockInter4.EXPECT().Foo(gomock.Any()).Return(3).Times(1),
		mockInter4.EXPECT().Bar(gomock.Any()).Return(4).Times(1),
	)
	testUser4 := &User{Inter: mockInter4}
	if n := testUser4.Use(); n != 3 {
		t.Errorf("result = %v", n)
	}
	if n := testUser4.Use(); n != 7 {
		t.Errorf("result = %v", n)
	}
}

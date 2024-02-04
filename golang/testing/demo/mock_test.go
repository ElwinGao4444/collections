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
// mockgen -package=demo . Inter > ./demo_mock.go
func TestGoMockSourceMode(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	// 先设置期望的返回结果
	mockInter := NewMockInter(mockCtrl)
	mockInter.EXPECT().Foo(0).Return(nil).Times(1)

	// 再调用方法
	testUser := &User{Inter: mockInter}
	err := testUser.Use()
	if err != nil {
		t.Errorf("result = %v", err)
	}
}

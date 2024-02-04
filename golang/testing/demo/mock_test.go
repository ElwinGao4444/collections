package demo

import "testing"

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
}

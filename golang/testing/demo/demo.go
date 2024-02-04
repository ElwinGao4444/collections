package demo

// 接口定义
type Inter interface {
	Foo(int) error
}

// 驱动类
type User struct {
	Inter Inter
}

// 接口调用
func (user *User) Use() error {
	return user.Inter.Foo(0)
}

// 多接口定义，用于说明GoMock的两种使用模式
type Inter2 interface {
	Foo2(bool) error
}

type Inter3 interface {
	Foo3(string) error
}

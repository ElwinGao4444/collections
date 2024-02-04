package demo

type Inter interface {
	Foo(int) error
	Bar(string) error
}

type Inter2 interface {
	Foo2(int) error
	Bar2(string) error
}

type Inter3 interface {
	Foo3(int) error
	Bar3(string) error
}

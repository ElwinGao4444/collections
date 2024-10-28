package main

import (
	"bytes"
	"fmt"

	"github.com/vmihailenco/msgpack"
)

func simple_marshal_unmashal() {
	type Item struct {
		Foo string
	}

	b, err := msgpack.Marshal(&Item{Foo: "bar"})
	if err != nil {
		panic(err)
	}

	var item Item
	err = msgpack.Unmarshal(b, &item)
	if err != nil {
		panic(err)
	}
	fmt.Println(item.Foo)
}

func as_array() {
	type Item struct {
		_msgpack struct{} `msgpack:",asArray"`
		Foo      string
		Bar      string
	}

	var buf bytes.Buffer
	enc := msgpack.NewEncoder(&buf)
	err := enc.Encode(&Item{Foo: "foo", Bar: "bar"})
	if err != nil {
		panic(err)
	}

	dec := msgpack.NewDecoder(&buf)
	v, err := dec.DecodeInterface()
	if err != nil {
		panic(err)
	}
	fmt.Println(v)
}

func map_string_interface() {
	in := map[string]interface{}{"foo": 1, "hello": "world"}
	b, err := msgpack.Marshal(in)
	if err != nil {
		panic(err)
	}

	var out map[string]interface{}
	err = msgpack.Unmarshal(b, &out)
	if err != nil {
		panic(err)
	}

	fmt.Println(out)
}

func omit_empty() {
	type Item struct {
		Foo string
		Bar string
	}

	item := &Item{
		Foo: "hello",
	}
	b, err := msgpack.Marshal(item)
	if err != nil {
		panic(err)
	}
	fmt.Printf("item: %q\n", b)

	type ItemOmitEmpty struct {
		_msgpack struct{} `msgpack:",omitempty"`
		Foo      string
		Bar      string
	}

	itemOmitEmpty := &ItemOmitEmpty{
		Foo: "hello",
	}
	b, err = msgpack.Marshal(itemOmitEmpty)
	if err != nil {
		panic(err)
	}
	fmt.Printf("item2: %q\n", b)
}

func decode_query() {
	type InItem struct {
		InFoo string
		InBar string
	}
	type Item struct {
		Foo    string
		Bar    string
		InItem InItem
	}

	b, err := msgpack.Marshal(&Item{
		Foo:    "foo",
		Bar:    "bar",
		InItem: InItem{InFoo: "in_foo", InBar: "in_bar"},
	})
	if err != nil {
		panic(err)
	}

	dec := msgpack.NewDecoder(bytes.NewBuffer(b))
	values, err := dec.Query("InItem.InBar")
	if err != nil {
		panic(err)
	}
	fmt.Println("Bar:", values)
}

func decode_query_array_map() {
	b, err := msgpack.Marshal([]map[string]interface{}{
		{"id": 123, "attrs": map[string]interface{}{"phone": 12345}},
		{"id": 543, "attrs": map[string]interface{}{"phone": 54321}},
	})
	if err != nil {
		panic(err)
	}

	dec := msgpack.NewDecoder(bytes.NewBuffer(b))
	values, err := dec.Query("*.attrs.phone")
	if err != nil {
		panic(err)
	}
	fmt.Println("phones are", values)

	dec.Reset(bytes.NewBuffer(b))
	values, err = dec.Query("1.attrs.phone")
	if err != nil {
		panic(err)
	}
	fmt.Println("2nd phone is", values[0])
}

func partial_unmarshal() {
	type FullData struct {
		ID      int
		Name    string
		Age     int
		Address string
	}

	type PartialData struct {
		Name string
		Age  int
	}

	fullData := FullData{ID: 1, Name: "John", Age: 30, Address: "123 Main St"}
	b, err := msgpack.Marshal(fullData)
	if err != nil {
		panic(err)
	}

	var partialData PartialData
	err = msgpack.Unmarshal(b, &partialData)
	if err != nil {
		panic(err)
	}

	fmt.Printf("Partial Data: %+v\n", partialData)
}

func main() {
	simple_marshal_unmashal()
	as_array()
	map_string_interface()
	omit_empty()
	decode_query()
	decode_query_array_map()
	partial_unmarshal()
}

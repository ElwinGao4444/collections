package main

// 启动方式：protoc --go_out=. message.proto && go run main.go message.pb.go

import (
	"encoding/json"
	"log"

	pb "google.golang.org/protobuf/proto"
)

func use_simple_persion() {
	var data []byte
	var err error
	var old_obj *SimplePerson
	var new_obj *SimplePerson

	// 定义一个结构体
	old_obj = &SimplePerson{Name: "foo", Male: true, Scores: []int32{60, 70, 80}}

	// 使用json序列化
	if data, err = json.Marshal(old_obj); err != nil {
		log.Fatal("json marshaling error: ", err)
	}
	log.Println("Json Marshal: ", string(data))

	// 使用pb序列化
	if data, err = pb.Marshal(old_obj); err != nil {
		log.Fatal("marshaling error: ", err)
	}
	log.Println("Marshal: ", data)

	// 使用pb反序列化
	new_obj = &SimplePerson{}
	if err = pb.Unmarshal(data, new_obj); err != nil {
		log.Fatal("unmarshaling error: ", err)
	}
	log.Println("Marshal: ", new_obj)

	// 监测序列化与反序列化的数据一致性
	if old_obj.GetName() != new_obj.GetName() {
		log.Fatalf("data mismatch %q != %q", old_obj.GetName(), new_obj.GetName())
	}
}

func use_complex_persion() {
	var data []byte
	var err error
	var old_obj *ComplexPerson
	var new_obj *ComplexPerson

	// 定义一个结构体
	old_obj = &ComplexPerson{
		Base: &PersonBase{Id: 1234, Name: "John Doe", Email: "jdoe@example.com"},
		Phones: []*ComplexPerson_PhoneNumber{ // 内嵌结构体的引用
			{Number: "555-4321", Type: PhoneType_PHONE_TYPE_HOME},
		},
	}

	// 使用json序列化
	if data, err = json.Marshal(old_obj); err != nil {
		log.Fatal("json marshaling error: ", err)
	}
	log.Println("Json Marshal: ", string(data))

	// 使用pb序列化
	if data, err = pb.Marshal(old_obj); err != nil {
		log.Fatal("marshaling error: ", err)
	}
	log.Println("Marshal: ", data)

	// 使用pb反序列化
	new_obj = &ComplexPerson{}
	if err = pb.Unmarshal(data, new_obj); err != nil {
		log.Fatal("unmarshaling error: ", err)
	}
	log.Println("Marshal: ", new_obj)
}

func main() {
	use_simple_persion()
	use_complex_persion()
}

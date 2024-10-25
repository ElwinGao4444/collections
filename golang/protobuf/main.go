package main

// 启动方式：protoc --go_out=. message.proto && go run main.go message.pb.go

import (
	"encoding/json"
	"fmt"
	"log"

	pb "google.golang.org/protobuf/proto"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
)

func use_simple_persion() {
	log.Println("================ Case[use_simple_persion] ================")
	var data []byte
	var err error
	var old_obj *SimplePerson
	var new_obj *SimplePerson

	// 定义一个结构体
	old_obj = &SimplePerson{Name: pb.String("foo"), Male: pb.Bool(true), Scores: []int32{60, 70, 80}}

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
	log.Println("================ Case[use_complex_persion] ================")
	var data []byte
	var err error
	var old_obj *ComplexPerson
	var new_obj *ComplexPerson

	// 定义一个结构体
	old_obj = &ComplexPerson{
		Base: &PersonBase{Id: pb.Int32(1234), Name: pb.String("John Doe"), Email: pb.String("jdoe@example.com")},
		Phones: []*ComplexPerson_PhoneNumber{ // 内嵌结构体的引用
			{Number: pb.String("555-4321"), Type: (*PhoneType)(pb.Int32(int32(PhoneType_PHONE_TYPE_HOME)))},
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

func message_merge() {
	log.Println("================ Case[message_merge] ================")

	// 根据实测结果：
	// 1. 非集合类型，新数据覆盖老数据
	// 2. 新字段为空值，不会覆盖老数据
	// 3. 集合类型，追加而非合并
	// 在edition版本中，由于所有的字段都是指针，所以不会出现0值的歧义问题
	// 但是在proto3中，字段为实际类型（比如bool），当新值赋值为0（比如false）值时，会导致pb认为该值是初始0值，从而不对老数据进行覆盖，导致出现奇奇怪怪的问题
	var obj1 = &SimplePerson{Name: pb.String("foo"), Male: pb.Bool(true), Scores: []int32{1, 2, 3}}
	var obj2 = &SimplePerson{Name: pb.String("bar"), Male: pb.Bool(false), Scores: []int32{4, 5, 6}}

	log.Println("obj1: ", obj1)
	log.Println("obj2: ", obj2)
	pb.Merge(obj1, obj2)
	log.Println("merge_obj1: ", obj1)
}

func message_extensions() {
	log.Println("================ Case[message_extensions] ================")
	var user_content UserContent
	var extData = []*SimplePerson{
		&SimplePerson{Name: pb.String("z3"), Male: pb.Bool(true), Scores: []int32{1, 3, 5}},
		&SimplePerson{Name: pb.String("l4"), Male: pb.Bool(false), Scores: []int32{2, 4, 6}},
	}
	pb.SetExtension(&user_content, E_Person, extData)
	fmt.Println("HasExtension:", pb.HasExtension(&user_content, E_Person))
	var extDataOut = pb.GetExtension(&user_content, E_Person).([]*SimplePerson)
	fmt.Println("GasExtension:", extDataOut)
	pb.RangeExtensions(&user_content, func(etype protoreflect.ExtensionType, edata any) bool {
		fmt.Println("RangeExtension:", edata)
		return true // true:继续循环，false:停止循环
	})
}

func main() {
	use_simple_persion()
	use_complex_persion()
	message_merge()
	message_extensions()
}

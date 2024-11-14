package main

// 启动方式：protoc --go_out=. message.proto && go run main.go message.pb.go

import (
	"encoding/json"
	"fmt"
	"log"
	"time"

	"google.golang.org/protobuf/proto"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
)

func to_json(i interface{}) string {
	var data []byte
	var err error
	if data, err = json.Marshal(&i); err != nil {
		log.Fatal("json marshaling error: ", err)
	}
	return string(data)
}

func use_simple_persion() {
	log.Println("================ Case[use_simple_persion] ================")

	// 定义一个结构体
	var old_obj = SimplePerson{Name: proto.String("foo"), Male: proto.Bool(true), Scores: []int32{60, 70, 80}}
	log.Println("old_obj:", to_json(old_obj))

	// 使用pb序列化
	data, _ := proto.Marshal(&old_obj)
	log.Println("marshal: ", data)

	// 使用pb反序列化
	var new_obj = SimplePerson{}
	proto.Unmarshal(data, &new_obj)
	log.Println("unmarshal: ", &new_obj)
}

func use_complex_persion() {
	log.Println("================ Case[use_complex_persion] ================")

	// 定义一个结构体
	var old_obj = ComplexPerson{
		Base: &PersonBase{Id: proto.Int32(1234), Name: proto.String("John Doe"), Email: proto.String("jdoe@example.com")},
		Phones: []*ComplexPerson_PhoneNumber{ // 内嵌结构体的引用
			{Number: proto.String("555-4321"), Type: (*PhoneType)(proto.Int32(int32(PhoneType_PHONE_TYPE_HOME)))},
		},
	}
	log.Println("old_obj:", to_json(old_obj))

	// 使用pb序列化
	data, _ := proto.Marshal(&old_obj)
	log.Println("marshal: ", data)

	// 使用pb反序列化
	var new_obj = ComplexPerson{}
	proto.Unmarshal(data, &new_obj)
	log.Println("unmarshal: ", &new_obj)
}

func use_partial_persion() {
	log.Println("================ Case[use_partial_persion] ================")

	// 定义一个结构体
	var old_obj = SimplePerson{Name: proto.String("foo"), Male: proto.Bool(true), Scores: []int32{60, 70, 80}}
	log.Println("old_obj:", to_json(old_obj))

	// 使用pb部分序列化
	data, _ := proto.MarshalOptions{AllowPartial: true}.Marshal(&old_obj)
	log.Println("marshal: ", data)

	// 使用pb部分反序列化
	var new_obj = PartialPerson{}
	proto.UnmarshalOptions{AllowPartial: true, DiscardUnknown: true}.Unmarshal(data, &new_obj)
	log.Println("unmarshal: ", &new_obj) // 通过观察结构体内容，可以体现DiscardUnknown的作用
}

func use_marshal_append_persion() {
	log.Println("================ Case[use_marshal_append_persion] ================")

	// 定义一个结构体
	var old_obj1 = SimplePerson{Name: proto.String("foo"), Male: proto.Bool(true), Scores: []int32{60, 70, 80}}
	var old_obj2 = SimplePerson{Name: proto.String("bar"), Male: proto.Bool(false), Scores: []int32{10, 20, 30}}
	log.Println("old_obj1:", to_json(old_obj1))
	log.Println("old_obj2:", to_json(old_obj2))

	// 使用pb追加序列化
	var data []byte
	data, _ = proto.MarshalOptions{}.MarshalAppend(data[:0], &old_obj1)
	log.Println("marshal1: ", data)
	data, _ = proto.MarshalOptions{}.MarshalAppend(data[:len(data)], &old_obj2)
	log.Println("marshal2: ", data)

	// 使用pb反序列化
	var new_obj = SimplePerson{}
	proto.Unmarshal(data, &new_obj)
	log.Println("unarshal: ", &new_obj)
}

func use_unmarshal_merge_persion() {
	log.Println("================ Case[use_unmarshal_merge_persion] ================")
	// 定义一个结构体
	var old_obj1 = SimplePerson{Name: proto.String("foo"), Male: proto.Bool(true), Scores: []int32{60, 70, 80}}
	var old_obj2 = SimplePerson{Name: proto.String("bar"), Male: proto.Bool(false), Scores: []int32{10, 20, 30}}
	log.Println("old_obj1:", to_json(old_obj1))
	log.Println("old_obj2:", to_json(old_obj2))

	// 使用pb序列化
	data1, _ := proto.MarshalOptions{}.Marshal(&old_obj1)
	log.Println("marshal1: ", data1)
	data2, _ := proto.MarshalOptions{}.Marshal(&old_obj2)
	log.Println("marshal2: ", data2)

	// 使用pb合并反序列化
	var new_obj = SimplePerson{}
	proto.UnmarshalOptions{Merge: true}.Unmarshal(data1, &new_obj)
	log.Println("unmarshal merge 1: ", &new_obj)
	proto.UnmarshalOptions{Merge: true}.Unmarshal(data2, &new_obj)
	log.Println("unmarshal merge 2: ", &new_obj)
}

func message_merge() {
	log.Println("================ Case[message_merge] ================")

	// 根据实测结果：
	// 1. 非集合类型，新数据覆盖老数据
	// 2. 新字段为空值，不会覆盖老数据
	// 3. 集合类型，追加而非合并
	// 在edition版本中，由于所有的字段都是指针，所以不会出现0值的歧义问题
	// 但是在proto3中，基础字段为非指针类型（比如bool而非*bool），当新值赋值为0（比如false）值时，会导致pb认为该值是初始0值，从而不对老数据进行覆盖
	var obj1 = SimplePerson{Name: proto.String("foo"), Male: proto.Bool(true), Scores: []int32{1, 2, 3}}
	var obj2 = SimplePerson{Name: proto.String("bar"), Male: proto.Bool(false), Scores: []int32{4, 5, 6}}

	log.Println("obj1: ", to_json(obj1))
	log.Println("obj2: ", to_json(obj2))
	proto.Merge(&obj1, &obj2)
	log.Println("merge_obj: ", to_json(obj1))
}

func message_extensions() {
	log.Println("================ Case[message_extensions] ================")
	var user_content UserContent
	var extData = []*SimplePerson{
		&SimplePerson{Name: proto.String("z3"), Male: proto.Bool(true), Scores: []int32{1, 3, 5}},
		&SimplePerson{Name: proto.String("l4"), Male: proto.Bool(false), Scores: []int32{2, 4, 6}},
	}
	proto.SetExtension(&user_content, E_Person, extData)
	fmt.Println("HasExtension:", proto.HasExtension(&user_content, E_Person))
	var extDataOut = proto.GetExtension(&user_content, E_Person).([]*SimplePerson)
	fmt.Println("GasExtension:", extDataOut)
	proto.RangeExtensions(&user_content, func(etype protoreflect.ExtensionType, edata any) bool {
		fmt.Println("RangeExtension:", edata)
		return true // true:继续循环，false:停止循环
	})
}

func benchmark_slice_map() {
	log.Println("================ Case[benchmark_slice_map] ================")
	var n = 100000
	var s = make([]int32, n)
	var m = make(map[int32]int32, n)
	for i := 0; i < n; i++ {
		s[i] = int32(i)
		m[int32(i)] = int32(i)
	}

	var data []byte
	var res = SpliceMapBenchmark{}
	var start time.Time

	start = time.Now()
	var bench_slice = SpliceMapBenchmark{DataSlice: s}
	data, _ = proto.Marshal(&bench_slice)
	log.Println("slice marshal:", time.Since(start))

	start = time.Now()
	proto.Unmarshal(data, &res)
	log.Println("slice unmarshal:", time.Since(start))

	start = time.Now()
	var bench_map = SpliceMapBenchmark{DataMap: m}
	data, _ = proto.Marshal(&bench_map)
	log.Println("map marshal:", time.Since(start))

	start = time.Now()
	proto.Unmarshal(data, &res)
	log.Println("map unmarshal:", time.Since(start))
}

func main() {
	use_simple_persion()
	use_complex_persion()
	use_partial_persion()
	use_marshal_append_persion()
	use_unmarshal_merge_persion()
	message_merge()
	message_extensions()
	benchmark_slice_map()
}

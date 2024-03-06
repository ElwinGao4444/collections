/*
// =====================================================================================
//
//       Filename:  fake_step_with_params.go
//
//    Description:  step测试类
//
//        Version:  1.0
//        Created:  03/05/2024 18:06:44
//       Compiler:  go1.21.1
//
// =====================================================================================
*/

package fake_step

type FakeStepWithParams struct {
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStepWithParams) Name() string {
	return "FakeStepWithParams"
}

func (step *FakeStepWithParams) Error() error {
	return nil
}

func (step *FakeStepWithParams) Before(input interface{}, params ...interface{}) error {
	step.Data = 0
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	for _, it := range params {
		if v, ok := it.(*int); ok {
			(*v)++
		}
	}
	step.BeforeCount++
	return nil
}

func (step *FakeStepWithParams) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
	var output int
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
			v++
			output = v
		}
	} else {
		output = 1
	}
	for _, it := range params {
		if v, ok := it.(*int); ok {
			(*v)++
		}
	}
	step.DoStepCount++
	return output, nil
}

func (step *FakeStepWithParams) After(input interface{}, params ...interface{}) error {
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	for _, it := range params {
		if v, ok := it.(*int); ok {
			(*v)++
		}
	}
	step.AfterCount++
	return nil
}

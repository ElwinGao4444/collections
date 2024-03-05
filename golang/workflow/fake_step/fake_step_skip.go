/*
// =====================================================================================
//
//       Filename:  fake_step_skip.go
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

type FakeStepSkip struct {
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStepSkip) Name() string {
	return "FakeStepSkip"
}

func (step *FakeStepSkip) Error() error {
	return nil
}

func (step *FakeStepSkip) Before(input interface{}, params ...interface{}) (bool, error) {
	step.Data = 0
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	step.BeforeCount++
	return false, nil
}

func (step *FakeStepSkip) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
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
	step.DoStepCount++
	return output, nil
}

func (step *FakeStepSkip) After(input interface{}, params ...interface{}) (bool, error) {
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	step.AfterCount++
	return true, nil
}

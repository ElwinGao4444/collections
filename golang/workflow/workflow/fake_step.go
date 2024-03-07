/*
// =====================================================================================
//
//       Filename:  fake_step.go
//
//    Description:  step测试类
//
//        Version:  1.0
//        Created:  03/05/2024 18:06:44
//       Compiler:  go1.21.1
//
// =====================================================================================
*/

package workflow

import (
	"time"
)

type FakeStep struct {
	BaseStep
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStep) PreProcess(input interface{}, params ...interface{}) (interface{}, error) {
	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			if v.Error() == "before" {
				return 0, v
			}
		case bool:
			if v == true {
				params[0] = false
				return 2, nil
			}
		}
	}
	step.BeforeCount++
	return nil, nil
}

func (step *FakeStep) Process(input interface{}, params ...interface{}) (interface{}, error) {
	switch v := input.(type) {
	case int:
		step.Data = v + 1
	}

	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			if v.Error() == "step" {
				return 0, v
			}
		case time.Duration:
			time.Sleep(v)
		}
	}

	step.DoStepCount++
	return step.Data, nil
}

func (step *FakeStep) PostProcess(input interface{}, result interface{}, params ...interface{}) error {
	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			if v.Error() == "after" {
				return v
			}
		}
	}
	step.AfterCount++
	return nil
}

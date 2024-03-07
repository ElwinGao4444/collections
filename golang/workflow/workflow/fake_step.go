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
	"context"
	"time"
)

type FakeStep struct {
	BaseStep
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStep) SetName(name string) StepInterface {
	step.BaseStep.SetName(name)
	return step
}

func (step *FakeStep) PreProcess(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	if len(shared) > 0 {
		switch v := shared[0].(type) {
		case error:
			if v.Error() == "before" {
				return 0, v
			}
		case bool:
			if v == true {
				shared[0] = false
				return 2, nil
			}
		}
	}
	step.BeforeCount++
	return nil, nil
}

func (step *FakeStep) Process(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	switch v := input.(type) {
	case int:
		step.Data = v + 1
	}

	if len(shared) > 0 {
		switch v := shared[0].(type) {
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

func (step *FakeStep) PostProcess(ctx context.Context, input interface{}, result interface{}, shared ...interface{}) error {
	if len(shared) > 0 {
		switch v := shared[0].(type) {
		case error:
			if v.Error() == "after" {
				return v
			}
		}
	}
	step.AfterCount++
	return nil
}

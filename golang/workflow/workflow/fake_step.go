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

import "fmt"

type FakeStep struct {
	StepInterface
	name        string
	err         error
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStep) Name() string {
	return "FakeStep"
}

func (step *FakeStep) SetName(name string) {
	step.name = name
}

func (step *FakeStep) Error() error {
	return nil
}

func (step *FakeStep) SetError(err error) {
	step.err = err
}

func (step *FakeStep) Before(input interface{}, params ...interface{}) error {
	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			if v.Error() == "before" {
				return v
			}
		}
	}
	step.BeforeCount++
	return nil
}

func (step *FakeStep) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
	switch v := input.(type) {
	case int:
		step.Data = v * 2
	}

	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			if v.Error() == "step" {
				return 0, v
			}
		}
	}

	step.DoStepCount++
	return step.Data, nil
}

func (step *FakeStep) After(input interface{}, params ...interface{}) error {
	if len(params) > 0 {
		switch v := params[0].(type) {
		case error:
			fmt.Println("debug: inner: ", v)
			if v.Error() == "after" {
				return v
			}
		}
	}
	step.AfterCount++
	return nil
}
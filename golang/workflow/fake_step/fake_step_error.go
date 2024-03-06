/*
// =====================================================================================
//
//       Filename:  fake_step_error.go
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

import "errors"

type FakeStepError struct {
	BeforeCount int
	DoStepCount int
	AfterCount  int
}

func (step *FakeStepError) Name() string {
	return "FakeStepError"
}

func (step *FakeStepError) Error() error {
	return nil
}

func (step *FakeStepError) Before(input interface{}, params ...interface{}) error {
	step.BeforeCount++
	return nil
}

func (step *FakeStepError) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
	step.DoStepCount++
	return nil, errors.New("make error")
}

func (step *FakeStepError) After(input interface{}, params ...interface{}) error {
	step.AfterCount++
	return nil
}

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

func (step *FakeStepError) Before(input interface{}, params ...interface{}) (bool, error) {
	step.BeforeCount++
	return true, nil
}

func (step *FakeStepError) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
	step.DoStepCount++
	return nil, errors.New("make error")
}

func (step *FakeStepError) After(input interface{}, params ...interface{}) (bool, error) {
	step.AfterCount++
	return true, nil
}

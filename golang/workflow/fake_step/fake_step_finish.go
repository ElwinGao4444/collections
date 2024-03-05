package fake_step

type FakeStepFinish struct {
	BeforeCount int
	DoStepCount int
	AfterCount  int
	Data        int
}

func (step *FakeStepFinish) Name() string {
	return "FakeStepFinish"
}

func (step *FakeStepFinish) Error() error {
	return nil
}

func (step *FakeStepFinish) Before(input interface{}, params ...interface{}) (bool, error) {
	step.Data = 0
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	step.BeforeCount++
	return true, nil
}

func (step *FakeStepFinish) DoStep(input interface{}, params ...interface{}) (interface{}, error) {
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

func (step *FakeStepFinish) After(input interface{}, params ...interface{}) (bool, error) {
	if input != nil {
		if v, ok := input.(int); ok {
			step.Data += v
		}
	}
	step.AfterCount++
	return false, nil
}

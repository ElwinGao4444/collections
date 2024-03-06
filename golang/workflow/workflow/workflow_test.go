/*
// =====================================================================================
//
//       Filename:  workflow_test.go
//
//    Description:
//
//        Version:  1.0
//        Created:  03/05/2024 18:06:44
//       Compiler:  go1.21.1
//
// =====================================================================================
*/

package workflow

import (
	"errors"
	"testing"
	"time"
	"strconv"

	"github.com/stretchr/testify/assert"
)

func TestWorkflowBasic(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	var stepList = wf.GetStepList()
	assert.Equal(t, len(stepList), 2, "stepList len")
	stepList = append(stepList, new(FakeStep))
	wf.SetStepList(stepList)
	for i, step := range(wf.GetStepList()) {
		step.Init(strconv.Itoa(i))
	}
	assert.Equal(t, len(wf.GetStepList()), 3, "get stepList")
	assert.Equal(t, wf.status, WORKINIT, "init status")
	assert.Equal(t, wf.currentStepIndex, -1, "init step index")
	assert.Equal(t, wf.CurrentStep(), nil, "init current step")
	result, _ := wf.Start(0)
	assert.Equal(t, result.(int), 3, "workflow result")
	var elapseSum time.Duration
	for i, step := range wf.GetStepList() {
		assert.Equal(t, step.Name(), strconv.Itoa(i), "step name")
		assert.Equal(t, step.Error(), nil, "step error")
		assert.Equal(t, step.Status(), STEPFINISH, "step status")
		assert.Equal(t, step.Result(), i+1, "step result")
		assert.Greater(t, step.Elapse(), time.Duration(0), "step elapse")
		assert.Equal(t, step.(*FakeStep).BeforeCount, 1, "fake step before")
		assert.Equal(t, step.(*FakeStep).DoStepCount, 1, "fake step do step")
		assert.Equal(t, step.(*FakeStep).AfterCount, 1, "fake step after")
		assert.Equal(t, step.(*FakeStep).Data, i+1, "fake step data")
		elapseSum += step.Elapse()
	}
	assert.Equal(t, wf.Status(), WORKFINISH, "workflow stat")
	assert.Greater(t, wf.Elapse(), elapseSum, "workflow elapse")
}

func TestWorkflowDoStep(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	wf.status = WORKRUNNING
	var input interface{} = 0
	for i := 0; i < 3; i++ {
		assert.Equal(t, wf.HasNext(), true, "has next")
		var step = wf.StepNext().(*FakeStep)
		assert.NotNil(t, step, nil, "get next step")
		result, err := wf.doStep(step, input)
		assert.Equal(t, result, i+1, "step result")
		assert.Equal(t, err, nil, "step err")
		assert.Equal(t, step.BeforeCount, 1, "fake step before")
		assert.Equal(t, step.DoStepCount, 1, "fake step do step")
		assert.Equal(t, step.AfterCount, 1, "fake step after")
		assert.Equal(t, step.Data, i+1, "fake step data")
		assert.Equal(t, wf.currentStepIndex, i, "current index")
		assert.Equal(t, wf.CurrentStep().Status(), STEPFINISH, "current step stat")
		assert.Equal(t, step.Error(), nil, "step error")
		assert.Equal(t, step.Status(), STEPFINISH, "step status")
		assert.Equal(t, step.Result(), i+1, "step result")
		input = result
	}
	assert.Equal(t, wf.HasNext(), false, "has next")
}

func TestWorkflowSkipAndError(t *testing.T) {
	var wf *Workflow
	var result interface{}
	var err error

	// before skip
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	result, err = wf.Start(nil, true)
	assert.Equal(t, result.(int), 3, "workflow step skip")
	assert.Equal(t, err, nil, "workflow step skip")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Data, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPSKIP, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result(), 2, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Data, 3, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), 3, "fake step")
	assert.Equal(t, wf.Status(), WORKFINISH, "workflow status")

	// before error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	result, err = wf.Start(nil, errors.New("before"))
	assert.Equal(t, result.(int), 0, "workflow step skip")
	assert.Equal(t, err.Error(), "before", "workflow step skip")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Data, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "before", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERRFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result(), 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Data, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERRFINISH, "workflow status")

	// do step error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	result, err = wf.Start(1, errors.New("step"))
	assert.Equal(t, result.(int), 0, "workflow step error")
	assert.Equal(t, err.Error(), "step", "workflow step error")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Data, 2, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "step", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERRFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result(), 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Data, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERRFINISH, "workflow status")

	// after error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	result, err = wf.Start(1, errors.New("after"))
	assert.Equal(t, result.(int), 2, "workflow step after error")
	assert.Equal(t, err.Error(), "after", "workflow step skip")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Data, 2, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "after", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERRFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result(), 2, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Data, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERRFINISH, "workflow status")
}

func TestWorkflowTimeout(t *testing.T) {
	var wf = new(Workflow).
		Init("test").
		SetTTL(time.Duration(100) * time.Millisecond).
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep))
	result, err := wf.Start(0, time.Duration(50)*time.Millisecond)
	assert.Equal(t, err.Error(), "workflow timeout", "workflow timeout")
	assert.Equal(t, result.(int), 2, "workflow timeout")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result(), 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), 2, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKTIMEOUTFINISH, "workflow timeout")
}

func TestWorkflowAsyncStep(t *testing.T) {
	var wf = new(Workflow).
		Init("test").
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep))
	for _, step := range wf.stepAsyncList {
		assert.Equal(t, step.Result(), nil, "fake step")
		assert.Equal(t, step.Error(), nil, "async step error")
		assert.Equal(t, step.Status(), STEPWAIT, "async step status")
	}
	wf.Start(0, time.Duration(20)*time.Millisecond)
	assert.Less(t, wf.elapse, time.Duration(25)*time.Millisecond, "fake step")
	for _, step := range wf.stepAsyncList {
		assert.Equal(t, step.Result(), 1, "async step result")
		assert.Equal(t, step.Error(), nil, "async step error")
		assert.Equal(t, step.Status(), STEPFINISH, "async step status")
	}
}

func BenchmarkDoStep(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	for i := 0; i < b.N; i++ {
		wf.doStep(step, 0)
	}
}

func BenchmarkWorkflow(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.SetStepList([]StepInterface{step, step, step, step, step})
	for i := 0; i < b.N; i++ {
		wf.Reset()
		wf.Start(nil)
	}
}

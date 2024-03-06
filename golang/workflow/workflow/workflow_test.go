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

	"github.com/stretchr/testify/assert"
)

func TestWorkflowBasic(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	var stepList = wf.GetStepList()
	assert.Equal(t, len(stepList), 2, "test get stepList")
	stepList = append(stepList, new(FakeStep))
	wf.SetStepList(stepList)
	assert.Equal(t, len(wf.GetStepList()), 3, "test get stepList")
	assert.Equal(t, wf.status, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStep(), nil, "test current step stat")
	result, _ := wf.Start(0)
	assert.Equal(t, result.(int), 3, "test workflow result")
	for i, step := range wf.GetStepList() {
		assert.Equal(t, step.(*FakeStep).BeforeCount, 1, "test fake step")
		assert.Equal(t, step.(*FakeStep).DoStepCount, 1, "test fake step")
		assert.Equal(t, step.(*FakeStep).AfterCount, 1, "test fake step")
		assert.Equal(t, step.(*FakeStep).Data, i+1, "test fake step")
	}
}

func TestWorkflowDetail(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.AppendStep(new(FakeStep)).AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	assert.Equal(t, len(wf.GetStepList()), 3, "test get stepList")
	assert.Equal(t, wf.status, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStep(), nil, "test current step stat")
	wf.status = WORKRUNNING
	wf.pipeData = 0
	for i := 0; i < 3; i++ {
		assert.Equal(t, wf.HasNext(), true, "test has next")
		var step = wf.StepNext().(*FakeStep)
		assert.Equal(t, wf.doStep(), nil, "test has do step")
		assert.Equal(t, step.BeforeCount, 1, "test fake step")
		assert.Equal(t, step.DoStepCount, 1, "test fake step")
		assert.Equal(t, step.AfterCount, 1, "test fake step")
		assert.Equal(t, step.Data, i+1, "test fake step")
		assert.Equal(t, wf.currentStepIndex, i, "test index")
		if i == 2 {
			assert.Equal(t, wf.Status(), WORKFINISH, "test current work stat")
		} else {
			assert.Equal(t, wf.Status(), WORKRUNNING, "test current work stat")
		}
		assert.Equal(t, wf.CurrentStep().Status(), STEPFINISH, "test current step stat")
	}
	assert.Equal(t, wf.HasNext(), false, "test has next")
}

func TestWorkflowSkipAndError(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf = new(Workflow).Init("test").AppendStep(step)

	var result interface{}
	var err error

	result, err = wf.Start(nil, errors.New("before"))
	assert.Equal(t, result.(error).Error(), "before", "test workflow step skip")
	assert.Equal(t, err, nil, "test workflow step skip")
	assert.Equal(t, step.BeforeCount, 0, "test fake step")
	assert.Equal(t, step.DoStepCount, 0, "test fake step")
	assert.Equal(t, step.AfterCount, 0, "test fake step")

	result, err = wf.Start(1, errors.New("step"))
	assert.Equal(t, result.(int), 0, "test workflow step error")
	assert.Equal(t, err.Error(), "step", "test workflow step error")
	assert.Equal(t, step.BeforeCount, 1, "test fake step")
	assert.Equal(t, step.DoStepCount, 0, "test fake step")
	assert.Equal(t, step.AfterCount, 0, "test fake step")

	result, err = wf.Start(1, errors.New("after"))
	assert.Equal(t, result.(error).Error(), "after", "test workflow step after error")
	assert.Equal(t, err, nil, "test workflow step skip")
	assert.Equal(t, step.BeforeCount, 2, "test fake step")
	assert.Equal(t, step.DoStepCount, 1, "test fake step")
	assert.Equal(t, step.AfterCount, 0, "test fake step")
}

func TestWorkflowTimeout(t *testing.T) {
	var wf = new(Workflow).
		Init("test").
		SetTTL(time.Duration(100) * time.Millisecond).
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep))
	result, err := wf.Start(0, time.Duration(50)*time.Millisecond)
	assert.Equal(t, err.Error(), "workflow timeout", "test workflow timeout")
	assert.Equal(t, result.(int), 2, "test workflow timeout")
}

func TestWorkflowAsyncStep(t *testing.T) {
	var wf = new(Workflow).
		Init("test").
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep))
	for _, step := range wf.stepAsyncList {
		assert.Equal(t, step.Result(), 0, "test fake step")
	}
	wf.Start(0, time.Duration(20)*time.Millisecond)
	assert.Less(t, wf.elapse, time.Duration(25)*time.Millisecond, "test fake step")
	for _, step := range wf.stepAsyncList {
		assert.Equal(t, step.Result(), 1, "test fake step")
	}
}

func BenchmarkWorkflow(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.SetStepList([]StepInterface{step, step, step, step, step})
	for i := 0; i < b.N; i++ {
		wf.Start(nil)
		wf.Reset()
	}
}

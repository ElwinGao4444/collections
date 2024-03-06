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
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestWorkflowBasic(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", 0, nil)
	wf.AppendStep(step).AppendStep(step)
	var stepList = wf.GetStepList()
	assert.Equal(t, len(stepList), 2, "test get stepList")
	stepList = append(stepList, step)
	wf.SetStepList(stepList)
	assert.Equal(t, len(wf.GetStepList()), 3, "test get stepList")
	assert.Equal(t, wf.status, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	result, _ := wf.Start(1)
	assert.Equal(t, result.(int), 0+1+1+2, "test workflow result")
	assert.Equal(t, step.BeforeCount, 3, "test fake step")
	assert.Equal(t, step.DoStepCount, 3, "test fake step")
	assert.Equal(t, step.AfterCount, 3, "test fake step")
	assert.Equal(t, step.Data, 4, "test fake step")
}

func TestWorkflowDetail(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", 0, nil)
	wf.AppendStep(step).AppendStep(step)
	var stepList = wf.GetStepList()
	assert.Equal(t, len(stepList), 2, "test get stepList")
	stepList = append(stepList, step)
	wf.SetStepList(stepList)
	assert.Equal(t, len(wf.GetStepList()), 3, "test get stepList")
	assert.Equal(t, wf.status, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	wf.status = WORKRUNNING
	wf.pipeData = 1
	var n = 1
	for i := 0; i < 3; i++ {
		assert.Equal(t, wf.HasNext(), true, "test has next")
		wf.StepNext()
		assert.Equal(t, wf.doStep(), nil, "test has do step")
		assert.Equal(t, step.BeforeCount, i+1, "test fake step")
		assert.Equal(t, step.DoStepCount, i+1, "test fake step")
		assert.Equal(t, step.AfterCount, i+1, "test fake step")
		assert.Equal(t, step.Data, n, "test fake step")
		assert.Equal(t, wf.currentStepIndex, i, "test index")
		if i == 2 {
			assert.Equal(t, wf.WorkflowStat(), WORKFINISH, "test current work stat")
		} else {
			assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
		}
		assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
		assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")
		n += n
	}
	assert.Equal(t, wf.HasNext(), false, "test has next")
}

func TestWorkflowSkipAndError(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", 0, nil)
	wf.AppendStep(step)

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

func TestWorkflowTimeDuration(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", 0, nil)
	wf.AppendStep(step).AppendStep(step).AppendStep(step)
	wf.Start(0)
	fmt.Println(wf.stepStatusList)
	fmt.Println(wf.stepElapseList)
	fmt.Println(wf.elapse)
}

func BenchmarkWorkflow(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", 0, nil)
	wf.SetStepList([]StepInterface{step, step, step, step, step})
	for i := 0; i < b.N; i++ {
		wf.Start(nil)
		wf.Reset()
	}
}

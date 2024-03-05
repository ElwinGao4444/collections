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
	"fmt"
	"testing"
	"time"

	. "workflow/fake_step"

	"github.com/stretchr/testify/assert"
)

func TestWorkflow(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	assert.Equal(t, wf.stat, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	wf.Ready()
	assert.Equal(t, wf.stat, WORKREADY, "test ready")
	wf.Start()
	assert.Equal(t, wf.stat, WORKRUNNING, "test start")
	for i := 0; i < 3; i++ {
		assert.Equal(t, wf.HasNext(), true, "test has next")
		wf.StepNext()
		assert.Equal(t, wf.DoStep(), nil, "test has do step")
		assert.Equal(t, step.BeforeCount, i+1, "test fake step")
		assert.Equal(t, step.DoStepCount, i+1, "test fake step")
		assert.Equal(t, step.AfterCount, i+1, "test fake step")
		assert.Equal(t, step.Data, i*3, "test fake step")
		assert.Equal(t, wf.currentStepIndex, i, "test index")
		if i == 2 {
			assert.Equal(t, wf.WorkflowStat(), WORKFINISH, "test current work stat")
		} else {
			assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
		}
		assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
		assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")
	}
	assert.Equal(t, wf.HasNext(), false, "test has next")
}

func TestWorkflowError(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var steperror *FakeStepError = new(FakeStepError)
	var wf *Workflow = new(Workflow)
	wf.Init("test", "RunWithRetryOnce")
	wf.AddStep(step)
	wf.AddStep(steperror)
	wf.AddStep(step)
	assert.Equal(t, wf.stat, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	wf.Ready()
	assert.Equal(t, wf.stat, WORKREADY, "test ready")
	wf.Start()
	assert.Equal(t, wf.stat, WORKRUNNING, "test start")

	// step 1
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, step.BeforeCount, 1, "test fake step")
	assert.Equal(t, step.DoStepCount, 1, "test fake step")
	assert.Equal(t, step.AfterCount, 1, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 0, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")

	// step 2
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, steperror.BeforeCount, 2, "test fake step")
	assert.Equal(t, steperror.DoStepCount, 2, "test fake step")
	assert.Equal(t, steperror.AfterCount, 0, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 1, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKERRFINISH, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPERRFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPERRFINISH, "test last step stat")

	assert.Equal(t, wf.HasNext(), false, "test has next")
}

func TestWorkflowSkip(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var stepskip *FakeStepSkip = new(FakeStepSkip)
	var wf *Workflow = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(stepskip)
	wf.AddStep(step)
	assert.Equal(t, wf.stat, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	wf.Ready()
	assert.Equal(t, wf.stat, WORKREADY, "test ready")
	wf.Start()

	// step 1
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, step.BeforeCount, 1, "test fake step")
	assert.Equal(t, step.DoStepCount, 1, "test fake step")
	assert.Equal(t, step.AfterCount, 1, "test fake step")
	assert.Equal(t, step.Data, 0, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 0, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")

	// step 2
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, stepskip.BeforeCount, 1, "test fake step")
	assert.Equal(t, stepskip.DoStepCount, 0, "test fake step")
	assert.Equal(t, stepskip.AfterCount, 0, "test fake step")
	assert.Equal(t, stepskip.Data, 1, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 1, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPSKIP, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPSKIP, "test last step stat")

	// step 3
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, step.BeforeCount, 2, "test fake step")
	assert.Equal(t, step.DoStepCount, 2, "test fake step")
	assert.Equal(t, step.AfterCount, 2, "test fake step")
	assert.Equal(t, step.Data, 3, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 2, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKFINISH, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")
}

func TestWorkflowFinish(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var stepfinish *FakeStepFinish = new(FakeStepFinish)
	var wf *Workflow = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(stepfinish)
	wf.AddStep(step)
	assert.Equal(t, wf.stat, WORKINIT, "test init")
	assert.Equal(t, wf.currentStepIndex, -1, "test index")
	assert.Equal(t, wf.CurrentStepStat(), STEPUNKNOWN, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPUNKNOWN, "test last step stat")
	wf.Ready()
	assert.Equal(t, wf.stat, WORKREADY, "test ready")
	wf.Start()

	// step 1
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, step.BeforeCount, 1, "test fake step")
	assert.Equal(t, step.DoStepCount, 1, "test fake step")
	assert.Equal(t, step.AfterCount, 1, "test fake step")
	assert.Equal(t, step.Data, 0, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 0, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKRUNNING, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")

	// step 2
	assert.Equal(t, wf.HasNext(), true, "test has next")
	wf.StepNext()
	assert.Equal(t, wf.DoStep(), nil, "test has do step")
	assert.Equal(t, stepfinish.BeforeCount, 1, "test fake step")
	assert.Equal(t, stepfinish.DoStepCount, 1, "test fake step")
	assert.Equal(t, stepfinish.AfterCount, 1, "test fake step")
	assert.Equal(t, stepfinish.Data, 3, "test fake step")
	assert.Equal(t, wf.currentStepIndex, 1, "test index")
	assert.Equal(t, wf.WorkflowStat(), WORKFINISH, "test current work stat")
	assert.Equal(t, wf.CurrentStepStat(), STEPFINISH, "test current step stat")
	assert.Equal(t, wf.LastStepStat(), STEPFINISH, "test last step stat")

	// step 3
	assert.Equal(t, wf.HasNext(), false, "test has next")
}

func TestWorkflowTimeDuration(t *testing.T) {
	var step *FakeStep = new(FakeStep)
	var steperror *FakeStepError = new(FakeStepError)
	var wf *Workflow
	var wfDuration time.Duration
	var stepDurationList []time.Duration

	// normal case
	wf = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.Ready()
	wf.Start()
	assert.Equal(t, wf.DoWorkflow(), nil, "test do workflow")

	wfDuration, stepDurationList = wf.TimeDuration()
	assert.Equal(t, len(stepDurationList), 3, "test")
	fmt.Println(wfDuration)
	fmt.Println(stepDurationList)

	// error case
	wf = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(steperror)
	wf.AddStep(step)
	wf.Ready()
	wf.Start()
	assert.Equal(t, wf.DoWorkflow(), nil, "test do workflow")

	wfDuration, stepDurationList = wf.TimeDuration()
	assert.Equal(t, len(stepDurationList), 2, "test")
	fmt.Println(wfDuration)
	fmt.Println(stepDurationList)
}

func TestWorkflowWithParams(t *testing.T) {
	var step *FakeStepWithParams = new(FakeStepWithParams)
	var wf *Workflow = new(Workflow)
	var i int = 0
	var p *int = &i
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.Ready()
	wf.Start()
	wf.DoWorkflow(p)

	assert.Equal(t, i, 15, "test")
}

func BenchmarkWorkflow(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test", "default")
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.AddStep(step)
	wf.Ready()
	wf.Start()
	for i := 0; i < b.N; i++ {
		wf.DoWorkflow()
		wf.Reset()
	}
}

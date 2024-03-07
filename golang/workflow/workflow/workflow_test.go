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
	"context"
	"errors"
	"strconv"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
)

func TestWorkflowBasic(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	wf.AppendStep(new(FakeStep).SetName("0")).AppendStep(new(FakeStep).SetName("1"))
	var stepList = wf.GetStepList()
	assert.Equal(t, len(stepList), 2, "stepList len")
	stepList = append(stepList, new(FakeStep).SetName("2"))
	wf.SetStepList(stepList)
	assert.Equal(t, len(wf.GetStepList()), 3, "get stepList")
	assert.Equal(t, wf.status, WORKINIT, "init status")
	assert.Equal(t, wf.currentStepIndex, -1, "init step index")
	assert.Equal(t, wf.CurrentStep(), nil, "init current step")
	ctx, _ := wf.Start(context.WithValue(context.Background(), "value", 1))
	assert.Equal(t, ctx.Value("value").(int), 4, "workflow result")
	for i, step := range wf.GetStepList() {
		assert.Equal(t, step.Name(), strconv.Itoa(i), "step name")
		assert.Equal(t, step.Error(), nil, "step error")
		assert.Equal(t, step.Status(), STEPFINISH, "step status")
		assert.Equal(t, step.Result().Value("value").(int), i+2, "step result")
		assert.Equal(t, step.(*FakeStep).BeforeCount, 1, "fake step before")
		assert.Equal(t, step.(*FakeStep).DoStepCount, 1, "fake step do step")
		assert.Equal(t, step.(*FakeStep).AfterCount, 1, "fake step after")
	}

	assert.Equal(t, wf.Status(), WORKFINISH, "workflow stat")
}

func TestWorkflowDoStep(t *testing.T) {
	var wf *Workflow = new(Workflow)
	wf.Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	wf.status = WORKRUNNING
	var ctx = context.WithValue(context.Background(), "value", 1)
	for i := 0; i < 3; i++ {
		assert.Equal(t, wf.HasNext(), true, "has next")
		var step = wf.NextStep().(*FakeStep)
		assert.NotNil(t, step, nil, "get next step")
		var err error
		ctx, err = wf.doStep(step, ctx)
		assert.Equal(t, ctx.Value("value").(int), i+2, "step result")
		assert.Equal(t, err, nil, "step err")
		assert.Equal(t, step.BeforeCount, 1, "fake step before")
		assert.Equal(t, step.DoStepCount, 1, "fake step do step")
		assert.Equal(t, step.AfterCount, 1, "fake step after")
		assert.Equal(t, wf.currentStepIndex, i, "current index")
		assert.Equal(t, wf.CurrentStep().Status(), STEPFINISH, "current step stat")
		assert.Equal(t, step.Error(), nil, "step error")
		assert.Equal(t, step.Status(), STEPFINISH, "step status")
		assert.Equal(t, step.Result().Value("value").(int), i+2, "step result")
	}
	assert.Equal(t, wf.HasNext(), false, "has next")
}

func TestWorkflowError(t *testing.T) {
	var wf *Workflow
	var ctx context.Context
	var err error

	// before error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	ctx = context.Background()
	ctx = context.WithValue(ctx, "value", 1)
	ctx = context.WithValue(ctx, "error", "PreProcess")
	ctx, err = wf.Start(ctx)
	assert.Equal(t, ctx.Value("value").(int), 1, "workflow step error")
	assert.Equal(t, err.Error(), "PreProcess Error", "workflow step error")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "PreProcess Error", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERROR, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result().Value("value").(int), 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERROR, "workflow status")

	// do step error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	ctx = context.WithValue(ctx, "error", "Process")
	ctx, err = wf.Start(ctx, 1, errors.New("step"))
	assert.Equal(t, ctx.Value("value").(int), 1, "workflow step error")
	assert.Equal(t, err.Error(), "Process Error", "workflow step error")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "Process Error", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERROR, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result().Value("value").(int), 1, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERROR, "workflow status")

	// after error
	wf = new(Workflow).Init("test").AppendStep(new(FakeStep)).AppendStep(new(FakeStep))
	ctx = context.WithValue(ctx, "error", "PostProcess")
	ctx, err = wf.Start(ctx, 1, errors.New("after"))
	assert.Equal(t, ctx.Value("value").(int), 2, "workflow step error")
	assert.Equal(t, err.Error(), "PostProcess Error", "workflow step skip")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).BeforeCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).DoStepCount, 1, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error().Error(), "PostProcess Error", "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPERROR, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result().Value("value").(int), 2, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).BeforeCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).DoStepCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).AfterCount, 0, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKERROR, "workflow status")
}

func TestWorkflowTimeout(t *testing.T) {
	var ctx context.Context
	var cancel context.CancelFunc
	var wf = new(Workflow).
		Init("test").
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep)).
		AppendStep(new(FakeStep))
	ctx = context.Background()
	ctx = context.WithValue(ctx, "value", 1)
	ctx = context.WithValue(ctx, "sleep", 50*time.Millisecond)
	ctx, cancel = context.WithTimeout(ctx, 100*time.Millisecond)
	defer cancel()
	ctx, err := wf.Start(ctx, 0, time.Duration(50)*time.Millisecond)
	assert.Equal(t, err.Error(), "workflow canceled", "workflow timeout")
	assert.Equal(t, ctx.Value("value").(int), 3, "workflow timeout")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Status(), STEPFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[0].(*FakeStep).Result().Value("value").(int), 2, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Status(), STEPFINISH, "fake step")
	assert.Equal(t, wf.GetStepList()[1].(*FakeStep).Result().Value("value").(int), 3, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Error(), nil, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Status(), STEPWAIT, "fake step")
	assert.Equal(t, wf.GetStepList()[2].(*FakeStep).Result(), nil, "fake step")
	assert.Equal(t, wf.Status(), WORKCANCEL, "workflow timeout")
}

func TestWorkflowAsyncStep(t *testing.T) {
	var wf = new(Workflow).
		Init("test").
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep)).
		AppendAsyncStep(new(FakeStep))
	for _, step := range wf.asyncStepList {
		assert.Equal(t, step.Result(), nil, "fake step")
		assert.Equal(t, step.Error(), nil, "async step error")
		assert.Equal(t, step.Status(), STEPWAIT, "async step status")
	}
	var ctx context.Context
	ctx = context.Background()
	ctx = context.WithValue(ctx, "value", 1)
	ctx = context.WithValue(ctx, "sleep", 20*time.Millisecond)
	wf.Start(ctx)
	assert.Less(t, wf.elapse, time.Duration(25)*time.Millisecond, "fake step")
	for _, step := range wf.asyncStepList {
		assert.Equal(t, step.Result().Value("value").(int), 2, "async step result")
		assert.Equal(t, step.Error(), nil, "async step error")
		assert.Equal(t, step.Status(), STEPFINISH, "async step status")
	}
}

func BenchmarkDoStep(b *testing.B) {
	b.ResetTimer()

	var step *FakeStep = new(FakeStep)
	var wf *Workflow = new(Workflow)
	wf.Init("test")
	var ctx = context.Background()
	for i := 0; i < b.N; i++ {
		wf.doStep(step, ctx, 0)
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
		wf.Start(context.Background(), nil)
	}
}

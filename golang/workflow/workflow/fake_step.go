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
	"errors"
	"time"
)

type FakeStep struct {
	BaseStep
	BeforeCount int
	DoStepCount int
	AfterCount  int
}

func (step *FakeStep) SetName(name string) StepInterface {
	step.BaseStep.SetName(name)
	return step
}

func (step *FakeStep) PreProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	if v := ctx.Value("error"); v != nil {
		if v.(string) == "PreProcess" {
			return ctx, errors.New("PreProcess Error")
		}
	}
	step.BeforeCount++
	return ctx, nil
}

func (step *FakeStep) Process(ctx context.Context, params ...interface{}) (context.Context, error) {
	if v := ctx.Value("error"); v != nil {
		if v.(string) == "Process" {
			return ctx, errors.New("Process Error")
		}
	}
	if v := ctx.Value("sleep"); v != nil {
		time.Sleep(v.(time.Duration))
	}
	if v := ctx.Value("value"); v != nil {
		ctx = context.WithValue(ctx, "value", v.(int)+1)
	}
	step.DoStepCount++
	return ctx, nil
}

func (step *FakeStep) PostProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	if v := ctx.Value("error"); v != nil {
		if v.(string) == "PostProcess" {
			return ctx, errors.New("PostProcess Error")
		}
	}
	step.AfterCount++
	return ctx, nil
}

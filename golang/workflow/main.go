package main

import (
	"context"
	"fmt"
	"time"
	"workflow/workflow"
)

type DemoStep struct {
	workflow.BaseStep
}

func (step *DemoStep) PreProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	fmt.Println("PreProcess: ", time.Now())
	return ctx, nil
}

func (step *DemoStep) Process(ctx context.Context, params ...interface{}) (context.Context, error) {
	fmt.Println("Process: ", time.Now())
	return ctx, nil
}

func (step *DemoStep) PostProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	fmt.Println("PostProcess: ", time.Now())
	return ctx, nil
}

func main() {
	var step0 = new(DemoStep)
	var step1 = step0.Copy().
		SetSimplePreProcess(func(ctx context.Context, params ...interface{}) (context.Context, error) {
			fmt.Println("Simple PreProcess")
			return ctx, nil
		}).
		SetSimpleProcess(func(ctx context.Context, params ...interface{}) (context.Context, error) {
			fmt.Println("Simple Process 1")
			return ctx, nil
		}).
		SetSimplePostProcess(func(ctx context.Context, params ...interface{}) (context.Context, error) {
			fmt.Println("Simple PostProcess")
			return ctx, nil
		})
	var step2 = step1.Copy().
		SetSimpleProcess(func(ctx context.Context, params ...interface{}) (context.Context, error) {
			fmt.Println("Simple Process 2")
			return ctx, nil
		})
	new(workflow.Workflow).Init("demo").
		AppendStep(step0).
		AppendStep(step1).
		AppendStep(step2).
		Start(context.Background())
	fmt.Println("vim-go")
}

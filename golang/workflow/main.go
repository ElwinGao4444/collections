package main

import (
	"context"
	"fmt"
	"workflow/workflow"
)

func main() {
	var step1 = new(workflow.BaseStep).
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
		AppendStep(step1).
		AppendStep(step2).
		Start(context.Background())
	fmt.Println("vim-go")
}

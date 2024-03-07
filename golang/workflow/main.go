package main

import (
	"fmt"
	"workflow/workflow"
)

func main() {
	wf = new(workflow.Workflow).Init("demo")
	wf.AppendStep(new(workflow.BaseStep))
	fmt.Println("vim-go")
}

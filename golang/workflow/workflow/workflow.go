/*
// =====================================================================================
//
//       Filename:  Workflow.go
//
//    Description:  并发任务流组件
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
	"sync"
	"time"
)

// =====================================================================================
//
//	       Type:  WorkStatus
//	Description:  状态信息（work与step共用状态，work看作是抽象step）
//
// =====================================================================================
type WorkStatus int

const WORKINIT WorkStatus = 0    // 工作流初始阶段
const WORKREADY WorkStatus = 1   // 工作流准备完毕
const WORKRUNNING WorkStatus = 2 // 工作流正在执行
const WORKFINISH WorkStatus = 3  // 工作流执行完成
const WORKERROR WorkStatus = 4   // 工作流执行失败
const WORKCANCEL WorkStatus = 5  // 工作流执行中止

type Workflow struct {
	name             string          // workflow名字
	currentStepIndex int             // 当前任务下标
	status           WorkStatus      // workflow状态信息
	elapse           time.Duration   // workflow整体耗时
	syncStepList     []StepInterface // workflow的同步step列表
	asyncStepList    []StepInterface // workflow的异步step列表
	waitGroup        *sync.WaitGroup // 等待异步step的同步机制
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Init
//  Description:  初始化workflow
// =====================================================================================
*/
func (wf *Workflow) Init(name string) *Workflow {
	wf.name = name
	wf.syncStepList = make([]StepInterface, 0)
	wf.asyncStepList = make([]StepInterface, 0)
	wf.waitGroup = new(sync.WaitGroup)
	wf.Reset()
	wf.status = WORKINIT
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Reset
//  Description:  重制workflow状态，以便重新执行workflow。
//                Reset不会删除已经注册进workflow的step，只会清空执行状态
// =====================================================================================
*/
func (wf *Workflow) Reset() *Workflow {
	wf.currentStepIndex = -1
	for _, step := range wf.syncStepList {
		step.SetStatus(STEPWAIT)
	}
	for _, step := range wf.asyncStepList {
		step.SetStatus(STEPWAIT)
	}
	wf.status = WORKREADY
	wf.elapse = 0
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Name
//  Description:
// =====================================================================================
*/
func (wf *Workflow) Name() string {
	return wf.name
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  AppendStep
//  Description:  追加step
// =====================================================================================
*/
func (wf *Workflow) AppendStep(step StepInterface) *Workflow {
	wf.syncStepList = append(wf.syncStepList, step)
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  AppendAsyncStep
//  Description:  追加异步step
// =====================================================================================
*/
func (wf *Workflow) AppendAsyncStep(step StepInterface) *Workflow {
	wf.asyncStepList = append(wf.asyncStepList, step)
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetStepList
//  Description:  设置同步step list，这个操作会覆盖workflow中已经注册的step
// =====================================================================================
*/
func (wf *Workflow) SetStepList(syncStepList []StepInterface) *Workflow {
	wf.syncStepList = syncStepList
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  GetStepList
//  Description:  获取同步step list
// =====================================================================================
*/
func (wf *Workflow) GetStepList() []StepInterface {
	return wf.syncStepList
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetAsyncStepList
//  Description:  设置异步step list，这个操作会覆盖workflow中已经注册的step
// =====================================================================================
*/
func (wf *Workflow) SetAyncStepList(asyncStepList []StepInterface) *Workflow {
	wf.asyncStepList = asyncStepList
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  GetAsyncStepList
//  Description:  获取同步step list
// =====================================================================================
*/
func (wf *Workflow) GetAsyncStepList() []StepInterface {
	return wf.asyncStepList
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Start
//  Description:
// =====================================================================================
*/
func (wf *Workflow) Start(ctx context.Context, params ...interface{}) (context.Context, error) {
	wf.Reset()
	wf.status = WORKRUNNING

	var workflowTimeBegin = time.Now()

	// 处理异步step
	for _, step := range wf.asyncStepList {
		wf.waitGroup.Add(1)
		go func(step StepInterface) {
			defer wf.waitGroup.Done()
			wf.doStep(step, ctx, params...)
		}(step)
	}

	// 处理同步step
	var err error
	for wf.HasNext() {
		if ctx, err = wf.doStep(wf.StepNext(), ctx, params...); err != nil {
			wf.status = WORKERROR
			return ctx, err
		}
		select {
		case <-ctx.Done():
			wf.status = WORKCANCEL
			return ctx, errors.New("workflow canceled")
		default:
			continue
		}
	}

	wf.waitGroup.Wait()
	wf.status = WORKFINISH
	wf.elapse = time.Since(workflowTimeBegin)
	return ctx, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  doStep
//  Description:  执行指定step
// =====================================================================================
*/
func (wf *Workflow) doStep(step StepInterface, ctx context.Context, params ...interface{}) (context.Context, error) {
	if step == nil {
		return nil, errors.New("currentStepIndex is nil")
	}

	var err error

	defer func() { step.SetResult(ctx) }()
	defer func() { step.SetError(err) }()

	// PreProcess
	step.SetStatus(STEPREADY)
	if ctx, err = step.PreProcess(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	// Process
	step.SetStatus(STEPRUNNING)
	if ctx, err = step.Process(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	// PostProcess
	step.SetStatus(STEPDONE)
	if ctx, err = step.PostProcess(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	step.SetStatus(STEPFINISH)
	return ctx, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  HasNext
//  Description:
// =====================================================================================
*/
func (wf *Workflow) HasNext() bool {
	if wf.status == WORKFINISH || wf.status == WORKERROR {
		return false
	}
	return wf.currentStepIndex < len(wf.syncStepList)-1
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  StepNext
//  Description:
// =====================================================================================
*/
func (wf *Workflow) StepNext() StepInterface {
	wf.currentStepIndex++
	return wf.CurrentStep()
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  CurrentStep
//  Description:
// =====================================================================================
*/
func (wf *Workflow) CurrentStep() StepInterface {
	if wf.currentStepIndex < 0 || wf.currentStepIndex >= len(wf.syncStepList) {
		return nil
	}
	return wf.syncStepList[wf.currentStepIndex]
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Status
//  Description:
// =====================================================================================
*/
func (wf *Workflow) Status() WorkStatus {
	return wf.status
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Elapse
//  Description:  获取上一个workflow的执行时间
// =====================================================================================
*/
//
func (wf *Workflow) Elapse() time.Duration {
	return wf.elapse
}

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
//  Description:  追加同步step
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
//  Description:  执行workflow，同步step和异步step会同时执行，在所有step执行完毕后返回
//                参数列表
//                ctx: workflow上下文，用于在step之内的3个阶段，与step之间传递数据，并
//                     将最后一个step，最后一个阶段返回的ctx作为整个workflow的返回信息
//                params: 自定义全局共享参数，该参数会在所有step的所有阶段共享，需要用
//                        户根据实际情况，对数据内容进行并发访问的保护
// =====================================================================================
*/
func (wf *Workflow) Start(ctx context.Context, params ...interface{}) (context.Context, error) {
	wf.Reset()
	wf.status = WORKRUNNING

	var workflowTimeBegin = time.Now()
	defer func() { wf.elapse = time.Since(workflowTimeBegin) }()

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
		if ctx, err = wf.doStep(wf.NextStep(), ctx, params...); err != nil {
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

	var waitGroupChan = make(chan struct{})
	go func() {
		wf.waitGroup.Wait()
		waitGroupChan <- struct{}{}
	}()
	select {
	case <-ctx.Done():
		wf.status = WORKCANCEL
		return ctx, errors.New("workflow canceled")
	case <-waitGroupChan:
		wf.status = WORKFINISH
	}
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

	var preProcess func(ctx context.Context, params ...interface{}) (context.Context, error)
	if preProcess = step.GetSimplePreProcess(); preProcess == nil {
		preProcess = step.PreProcess
	}
	var process func(ctx context.Context, params ...interface{}) (context.Context, error)
	if process = step.GetSimpleProcess(); process == nil {
		process = step.Process
	}
	var postProcess func(ctx context.Context, params ...interface{}) (context.Context, error)
	if postProcess = step.GetSimplePostProcess(); postProcess == nil {
		postProcess = step.PostProcess
	}

	// PreProcess
	step.SetStatus(STEPREADY)
	if ctx, err = preProcess(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	// Process
	step.SetStatus(STEPRUNNING)
	if ctx, err = process(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	// PostProcess
	step.SetStatus(STEPDONE)
	if ctx, err = postProcess(ctx, params...); err != nil {
		step.SetStatus(STEPERROR)
		return ctx, err
	}

	step.SetStatus(STEPFINISH)
	return ctx, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  HasNext
//  Description:  判断workflow的pipeline是否完成
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
//         Name:  CurrentStep
//  Description:  获取当前step
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
//         Name:  NextStep
//  Description:  获取下一个step
// =====================================================================================
*/
func (wf *Workflow) NextStep() StepInterface {
	wf.currentStepIndex++
	return wf.CurrentStep()
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Status
//  Description:  获取workflow的当前状态
// =====================================================================================
*/
func (wf *Workflow) Status() WorkStatus {
	return wf.status
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Elapse
//  Description:  获取workflow的总体执行时间
// =====================================================================================
*/
//
func (wf *Workflow) Elapse() time.Duration {
	return wf.elapse
}

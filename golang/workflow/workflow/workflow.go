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

	"github.com/golang/glog"
)

// =====================================================================================
//
//	       Type:  WorkStatus
//	Description:  状态信息（work与step共用状态，work看作是抽象step）
//
// =====================================================================================
type WorkStatus int

const WORKINIT WorkStatus = 0          // 工作流初始阶段
const WORKREADY WorkStatus = 1         // 工作流准备完毕
const WORKRUNNING WorkStatus = 2       // 工作流正在执行
const WORKFINISH WorkStatus = 3        // 工作流执行完成
const WORKERRFINISH WorkStatus = 4     // 工作流执行失败
const WORKTIMEOUTFINISH WorkStatus = 5 // 工作流执行超时
const WORKUNKNOWN WorkStatus = 6       // 未知状态（保留状态）

type Workflow struct {
	// workflow名字
	name string
	// 当前任务下标
	currentStepIndex int
	// workflow的step列表
	stepList []StepInterface
	// workflow的异步step列表
	stepAsyncList []StepInterface
	// workflow自身状态信息
	status WorkStatus
	// workflow整体耗时
	elapse time.Duration
	// workflow超时控制
	ttl time.Duration
	// 带重试策略的step执行函数
	retryPolicy func(func() error) error
	// 用户处理并发step的同步机制
	waitGroup *sync.WaitGroup
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Init
//  Description:  初始化workflow
//                name：workflow的名字
//                ttl：超时控制，0值表示不做超时控制
//                retryPolicy：重试策略，可以使用预制方法，也可以自定义，不重试可以传nil
// =====================================================================================
*/
func (wf *Workflow) Init(name string) *Workflow {
	wf.name = name
	wf.ttl = 0
	wf.retryPolicy = wf.NoRetry
	wf.stepList = make([]StepInterface, 0)
	wf.stepAsyncList = make([]StepInterface, 0)
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
	for _, step := range wf.stepList {
		step.SetStatus(STEPWAIT)
		step.SetElapse(0)
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
	step.SetStatus(STEPWAIT)
	wf.stepList = append(wf.stepList, step)
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  AppendAsyncStep
//  Description:  追加异步step
// =====================================================================================
*/
func (wf *Workflow) AppendAsyncStep(step StepInterface) *Workflow {
	step.SetStatus(STEPWAIT)
	wf.stepAsyncList = append(wf.stepAsyncList, step)
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetStepList
//  Description:  设置同步step list，这个操作会覆盖workflow中已经注册的step
// =====================================================================================
*/
func (wf *Workflow) SetStepList(stepList []StepInterface) *Workflow {
	wf.stepList = stepList
	for i, _ := range stepList {
		wf.stepList[i].SetStatus(STEPWAIT)
		wf.stepList[i].SetElapse(0)
	}
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  GetStepList
//  Description:  获取同步step list
// =====================================================================================
*/
func (wf *Workflow) GetStepList() []StepInterface {
	return wf.stepList
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  Start
//  Description:
// =====================================================================================
*/
func (wf *Workflow) Start(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	wf.Reset()
	wf.status = WORKRUNNING

	var workflowTimeBegin = time.Now()

	// 处理异步step
	for _, step := range wf.stepAsyncList {
		wf.waitGroup.Add(1)
		go func(step StepInterface) {
			defer wf.waitGroup.Done()
			wf.doStep(step, ctx, input, shared...)
		}(step)
	}

	// 处理同步step
	var result interface{}
	var err error
	for wf.HasNext() {
		if result, err = wf.doStep(wf.StepNext(), ctx, input, shared...); err != nil {
			wf.status = WORKERRFINISH
			return result, err
		}
		wf.elapse = time.Since(workflowTimeBegin)
		if wf.ttl > 0 && wf.elapse > wf.ttl {
			wf.status = WORKTIMEOUTFINISH
			return result, errors.New("workflow timeout")
		}
		input = result
	}

	wf.waitGroup.Wait()
	wf.status = WORKFINISH
	wf.elapse = time.Since(workflowTimeBegin)
	return result, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  doStep
//  Description:  执行指定step
// =====================================================================================
*/
func (wf *Workflow) doStep(step StepInterface, ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	if step == nil {
		glog.Errorf("currentStepIndex[%d] is outof range[%d]", wf.currentStepIndex, len(wf.stepList))
		return nil, errors.New("currentStepIndex is outof range")
	}

	// 创建step单次执行逻辑闭包
	var err error
	var result interface{} = nil
	defer func() { step.SetError(err) }()
	defer func() { step.SetResult(result) }()
	stepClosure := func() error {
		// do before
		step.SetStatus(STEPREADY)
		result, err = step.PreProcess(ctx, input, shared...)
		if err != nil {
			step.SetStatus(STEPERROR)
			return err
		}
		if result != nil {
			step.SetStatus(STEPSKIP)
			return nil
		}

		// do step
		step.SetStatus(STEPRUNNING)
		if result, err = step.Process(ctx, input, shared...); err != nil {
			step.SetStatus(STEPERROR)
			return err
		}

		// do after
		step.SetStatus(STEPDONE)
		if err = step.PostProcess(ctx, input, result, shared...); err != nil {
			step.SetStatus(STEPERROR)
			return err
		}

		step.SetStatus(STEPFINISH)
		return nil
	}

	// 基于重试策略执行step
	timeBegin := time.Now()
	defer func() { step.SetElapse(time.Since(timeBegin)) }()
	if err := wf.retryPolicy(stepClosure); err != nil {
		step.SetStatus(STEPERRFINISH)
		return result, err
	}
	return result, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  HasNext
//  Description:
// =====================================================================================
*/
func (wf *Workflow) HasNext() bool {
	if wf.status == WORKFINISH || wf.status == WORKERRFINISH {
		return false
	}
	return wf.currentStepIndex < len(wf.stepList)-1
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
//         Name:  Status
//  Description:
// =====================================================================================
*/
func (wf *Workflow) Status() WorkStatus {
	return wf.status
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  CurrentStep
//  Description:
// =====================================================================================
*/
func (wf *Workflow) CurrentStep() StepInterface {
	if wf.currentStepIndex < 0 || wf.currentStepIndex >= len(wf.stepList) {
		return nil
	}
	return wf.stepList[wf.currentStepIndex]
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

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetTTL
//  Description:  设置workflow超时报警
// =====================================================================================
*/
//
func (wf *Workflow) SetTTL(ttl time.Duration) *Workflow {
	wf.ttl = ttl
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetTTL
//  Description:  设置workflow超时报警
// =====================================================================================
*/
//
func (wf *Workflow) SetRetryPolicy(retryPolicy func(func() error) error) *Workflow {
	wf.retryPolicy = retryPolicy
	if wf.retryPolicy == nil {
		wf.retryPolicy = wf.NoRetry
	}
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  NoRetry
//  Description:
// =====================================================================================
*/
func (wf *Workflow) NoRetry(fun func() error) error {
	return fun()
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  runWithRetryOnce
//  Description:
// =====================================================================================
*/
func (wf *Workflow) RetryOnce(fun func() error) error {
	if err := fun(); err != nil {
		return fun()
	}
	return nil
}

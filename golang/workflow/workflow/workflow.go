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
	// workflow每项任务的状态列表
	stepStatusList []StepStatus
	// workflow每项任务的耗时信息
	stepElapseList []time.Duration
	// workflow自身状态信息
	stat WorkStatus
	// workflow整体耗时
	workflowElapse time.Duration
	// workflow超时控制
	ttl time.Duration
	// 带重试策略的step执行函数
	retryPolicy func(func() error) error
	// 存储相邻step之间的管道数据
	pipeData interface{}
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
func (wf *Workflow) Init(name string, ttl time.Duration, retryPolicy func(func() error) error) *Workflow {
	wf.name = name
	wf.ttl = ttl
	wf.retryPolicy = retryPolicy
	if wf.retryPolicy == nil {
		wf.retryPolicy = wf.NoRetry
	}
	wf.stepList = make([]StepInterface, 0)
	wf.Reset()
	wf.stat = WORKINIT
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
	wf.stepStatusList = make([]StepStatus, len(wf.stepList))
	wf.stepElapseList = make([]time.Duration, len(wf.stepList))
	wf.workflowElapse = 0
	wf.stat = WORKREADY
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
	wf.stepList = append(wf.stepList, step)
	wf.stepStatusList = append(wf.stepStatusList, STEPWAIT)
	wf.stepElapseList = append(wf.stepElapseList, 0)
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetStepList
//  Description:  设置step list，这个操作会覆盖workflow中已经注册的step
// =====================================================================================
*/
func (wf *Workflow) SetStepList(stepList []StepInterface) *Workflow {
	wf.stepList = stepList
	wf.stepStatusList = make([]StepStatus, len(stepList))
	wf.stepElapseList = make([]time.Duration, len(stepList))
	for i, _ := range stepList {
		wf.stepStatusList[i] = STEPWAIT
		wf.stepElapseList[i] = 0
	}
	return wf
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetStepList
//  Description:  设置step list，这个操作会覆盖workflow中已经注册的step
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
func (wf *Workflow) Start(input interface{}, params ...interface{}) (interface{}, error) {
	if len(wf.stepList) == 0 {
		glog.Errorf("name[%s] no step in workflow", wf.name)
		return nil, errors.New("no step in workflow")
	}
	wf.Reset()
	wf.stat = WORKRUNNING

	var workflowTimeBegin = time.Now()
	wf.pipeData = input
	for wf.HasNext() {
		wf.StepNext()
		var stepTimeBegin = time.Now()
		if err := wf.doStep(params...); err != nil {
			return wf.pipeData, err
		}
		wf.stepElapseList[wf.CurrentStep()] = time.Since(stepTimeBegin)
		wf.workflowElapse = time.Since(workflowTimeBegin)
		if wf.ttl > 0 && wf.workflowElapse > wf.ttl {
			wf.stat = WORKTIMEOUTFINISH
			return wf.pipeData, errors.New("workflow timeout quit")
		}
	}
	return wf.pipeData, nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  doStep
//  Description:
// =====================================================================================
*/
func (wf *Workflow) doStep(params ...interface{}) error {
	// 参数校验
	if wf.stat == WORKFINISH || wf.stat == WORKERRFINISH || wf.stat == WORKTIMEOUTFINISH {
		glog.Error("workflow has been finished")
		return errors.New("workflow has been finished")
	}
	if wf.currentStepIndex >= len(wf.stepList) {
		glog.Errorf("currentStepIndex[%d] is outof range[%d]", wf.currentStepIndex, len(wf.stepList))
		return errors.New("currentStepIndex is outof range")
	}

	// 创建step单次执行逻辑
	stepClosure := func() error {
		timeBegin := time.Now()

		// do before
		wf.stepStatusList[wf.currentStepIndex] = STEPREADY
		if err := wf.stepList[wf.currentStepIndex].Before(wf.pipeData, params...); err != nil {
			wf.stepStatusList[wf.currentStepIndex] = STEPSKIP
			wf.pipeData = err
			return nil
		}

		// do step
		wf.stepStatusList[wf.currentStepIndex] = STEPRUNNING
		var err error
		if wf.pipeData, err = wf.stepList[wf.currentStepIndex].DoStep(wf.pipeData, params...); err != nil {
			wf.stepStatusList[wf.currentStepIndex] = STEPERROR
			return err
		}

		// do after
		wf.stepStatusList[wf.currentStepIndex] = STEPDONE
		if err := wf.stepList[wf.currentStepIndex].After(wf.pipeData, params...); err != nil {
			wf.stepStatusList[wf.currentStepIndex] = STEPERROR
			wf.pipeData = err
			return nil
		}

		wf.stepStatusList[wf.currentStepIndex] = STEPFINISH

		wf.stepElapseList[wf.currentStepIndex] = time.Since(timeBegin)
		return nil
	}

	// 基于重试策略执行step
	if err := wf.retryPolicy(stepClosure); err != nil {
		wf.stepStatusList[wf.currentStepIndex] = STEPERRFINISH
		wf.stat = WORKERRFINISH
		return err
	}

	if wf.currentStepIndex == len(wf.stepList)-1 {
		wf.stat = WORKFINISH
	}

	return nil
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  HasNext
//  Description:
// =====================================================================================
*/
func (wf *Workflow) HasNext() bool {
	if wf.stat == WORKFINISH || wf.stat == WORKERRFINISH {
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
func (wf *Workflow) StepNext() {
	wf.currentStepIndex++
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  WorkflowStat
//  Description:
// =====================================================================================
*/
func (wf *Workflow) WorkflowStat() WorkStatus {
	return wf.stat
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  CurrentStep
//  Description:
// =====================================================================================
*/
func (wf *Workflow) CurrentStep() int {
	return wf.currentStepIndex
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  CurrentStepStat
//  Description:  获取当前step的状态，如果workflow处于非运行状态，则返回STEPUNKNOWN
// =====================================================================================
*/
func (wf *Workflow) CurrentStepStat() StepStatus {
	if wf.currentStepIndex < 0 {
		return STEPUNKNOWN
	}
	return wf.stepStatusList[wf.currentStepIndex]
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  GetAllStepElapse
//  Description:  workflow整体的耗时，及每个step的耗时
// =====================================================================================
*/
func (wf *Workflow) GetAllStepElapse() (time.Duration, []time.Duration) {
	return wf.workflowElapse, wf.stepElapseList[:wf.currentStepIndex+1]
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  LastStepStat
//  Description:
// =====================================================================================
*/
func (wf *Workflow) LastStepStat() StepStatus {
	if wf.currentStepIndex < 0 {
		return STEPUNKNOWN
	}
	if wf.currentStepIndex >= len(wf.stepList) {
		return wf.stepStatusList[len(wf.stepList)-1]
	} else {
		return wf.stepStatusList[wf.currentStepIndex]
	}
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  GetWorkflowElapse
//  Description:  获取上一个workflow的执行时间
// =====================================================================================
*/
//
func (wf *Workflow) GetWorkflowElapse() time.Duration {
	return wf.workflowElapse
}

/*
// ===  FUNCTION  ======================================================================
//         Name:  SetTimeoutWarning
//  Description:  设置workflow超时报警
// =====================================================================================
*/
//
func (wf *Workflow) SetTimeoutWarning(ttl time.Duration) {
	wf.ttl = ttl
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

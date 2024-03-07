/*
// =====================================================================================
//
//       Filename:  step_interface.go
//
//    Description:  step接口类
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
	"time"
)

type StepStatus int

const STEPWAIT StepStatus = 0      // 等待执行
const STEPREADY StepStatus = 1     // 任务开始，准备执行PreProcess()
const STEPRUNNING StepStatus = 2   // PreProcess执行完成，开始执行任务
const STEPDONE StepStatus = 3      // 任务执行完成，准备执行PostProcess()
const STEPSKIP StepStatus = 4      // 任务跳过，直接进入下一步
const STEPERROR StepStatus = 5     // 在任何阶段执行失败，都会进入STEPERROR状态
const STEPFINISH StepStatus = 6    // 任务经过重试，最终完成
const STEPERRFINISH StepStatus = 7 // 任务经过重试，最终失败
const STEPUNKNOWN StepStatus = 8   // 未知状态

/*
// ===  INTERFACE  =====================================================================
//         Name:  StepInterface
//  Description:
// =====================================================================================
*/
type StepInterface interface {
	// step的名字
	Name() string
	SetName(name string) StepInterface

	// step执行后的错误信息
	Error() error
	SetError(error) StepInterface

	// step状态信息
	Status() StepStatus
	SetStatus(StepStatus) StepInterface

	// step执行结果
	Result() interface{}
	SetResult(interface{}) StepInterface

	// step执行时间
	Elapse() time.Duration
	SetElapse(time.Duration) StepInterface

	// ===  FUNCTION  ======================================================================
	//         Name:  PreProcess
	//  Description:  step前置操作
	//                参数：input - 输入参数
	//                      shared - 自定义参数
	//                返回值: interface{}: 非空则跳过当前step，并以该interface{}作为下一个step的input
	//                        error: 如果error不为nil，则终止整个workflow
	// =====================================================================================
	PreProcess(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error)

	// ===  FUNCTION  ======================================================================
	//         Name:  Process
	//  Description:  step核心过程
	//                参数: input - 输入参数
	//                      shared - 自定义参数
	//                返回值: interface{} - 当前step的输出信息，会传递给下一个step作为input
	//                        error - step执行的错误信息，当error不为nil时，返回结果不置信
	// =====================================================================================
	Process(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error)

	// ===  FUNCTION  ======================================================================
	//         Name:  PostProcess
	//  Description:  step后置操作
	//                参数: input - 输入参数
	//                      result - Process的返回结果
	//                      shared - 自定义参数
	//                返回值: 如果error不为nil，则终止整个workflow，且step返回的结果不置信
	// =====================================================================================
	PostProcess(ctx context.Context, input interface{}, result interface{}, shared ...interface{}) error
}

type BaseStep struct {
	StepInterface
	name             string
	err              error
	status           StepStatus
	result           interface{}
	elapse           time.Duration
	anonymousProcess func(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error)
}

func (step *BaseStep) Name() string {
	return step.name
}

func (step *BaseStep) SetName(name string) StepInterface {
	step.name = name
	return step
}

func (step *BaseStep) Error() error {
	return step.err
}

func (step *BaseStep) SetError(err error) StepInterface {
	step.err = err
	return step
}

func (step *BaseStep) Status() StepStatus {
	return step.status
}

func (step *BaseStep) SetStatus(status StepStatus) StepInterface {
	step.status = status
	return step
}

func (step *BaseStep) Result() interface{} {
	return step.result
}

func (step *BaseStep) SetResult(result interface{}) StepInterface {
	step.result = result
	return step
}

func (step *BaseStep) Elapse() time.Duration {
	return step.elapse
}

func (step *BaseStep) SetElapse(elapse time.Duration) StepInterface {
	step.elapse = elapse
	return step
}

func (step *BaseStep) PreProcess(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	return false, nil
}

func (step *BaseStep) Process(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error) {
	return nil, nil
}

func (step *BaseStep) PostProcess(ctx context.Context, input interface{}, result interface{}, shared ...interface{}) error {
	return nil
}

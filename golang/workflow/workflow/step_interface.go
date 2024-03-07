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
)

type StepStatus int

const STEPWAIT StepStatus = 0    // 等待执行
const STEPREADY StepStatus = 1   // 任务开始，准备执行PreProcess()
const STEPRUNNING StepStatus = 2 // PreProcess执行完成，开始执行任务
const STEPDONE StepStatus = 3    // 任务执行完成，准备执行PostProcess()
const STEPFINISH StepStatus = 4  // 任务经过重试，最终完成
const STEPERROR StepStatus = 5   // 在任何阶段执行失败，都会进入STEPERROR状态

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

	// step的执行状态
	Status() StepStatus
	SetStatus(StepStatus) StepInterface

	// step的错误信息
	Error() error
	SetError(error) StepInterface

	// step的执行结果
	Result() context.Context
	SetResult(context.Context) StepInterface

	// ===  FUNCTION  ======================================================================
	//         Name:  PreProcess
	//  Description:  step前置操作
	//                参数: ctx: 上一个step返回的上下文信息
	//                      params - 自定义全局共享数据信息
	//                返回值: context: PreProcess的执行结果
	//                        error: 如果error不为nil，则终止整个workflow
	// =====================================================================================
	PreProcess(ctx context.Context, params ...interface{}) (context.Context, error)

	// ===  FUNCTION  ======================================================================
	//         Name:  Process
	//  Description:  step核心过程
	//                参数: ctx: PreProcess返回的上下文信息
	//                      params - 自定义全局共享数据信息
	//                返回值: context: Process的执行结果
	//                        error: 如果error不为nil，则终止整个workflow
	// =====================================================================================
	Process(ctx context.Context, params ...interface{}) (context.Context, error)

	// ===  FUNCTION  ======================================================================
	//         Name:  PostProcess
	//  Description:  step后置操作
	//                参数: ctx: Process返回的上下文信息
	//                      params - 自定义全局共享数据信息
	//                返回值: context: PostProcess的执行结果
	//                        error: 如果error不为nil，则终止整个workflow
	// =====================================================================================
	PostProcess(ctx context.Context, params ...interface{}) (context.Context, error)
}

// ===  注意  ======================================================================
// 由于golang并不能完美实现“继承”功能，所以，如果想通过链式调用使用SetXXX()方法，
// 就必须自己实现一遍SetXXX方法，而不能基于BaseStep的既有实现，否则会产生非预期的错误
// =================================================================================
type BaseStep struct {
	StepInterface
	name             string
	status           StepStatus
	result           context.Context
	err              error
	anonymousProcess func(ctx context.Context, input interface{}, shared ...interface{}) (interface{}, error)
}

func (step *BaseStep) Name() string {
	return step.name
}

func (step *BaseStep) SetName(name string) StepInterface {
	step.name = name
	return step
}

func (step *BaseStep) Result() context.Context {
	return step.result
}

func (step *BaseStep) SetResult(result context.Context) StepInterface {
	step.result = result
	return step
}

func (step *BaseStep) Status() StepStatus {
	return step.status
}

func (step *BaseStep) SetStatus(status StepStatus) StepInterface {
	step.status = status
	return step
}

func (step *BaseStep) Error() error {
	return step.err
}

func (step *BaseStep) SetError(err error) StepInterface {
	step.err = err
	return step
}

func (step *BaseStep) PreProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	return ctx, nil
}

func (step *BaseStep) Process(ctx context.Context, params ...interface{}) (context.Context, error) {
	return ctx, nil
}

func (step *BaseStep) PostProcess(ctx context.Context, params ...interface{}) (context.Context, error) {
	return ctx, nil
}

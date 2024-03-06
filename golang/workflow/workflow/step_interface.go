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

type StepStatus int

const STEPWAIT StepStatus = 0      // 等待调度
const STEPREADY StepStatus = 1     // 上调度，准备执行Before()
const STEPRUNNING StepStatus = 2   // Before执行完成，开始执行任务
const STEPDONE StepStatus = 3      // 任务执行完成，准备执行After()
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

	// step执行后的错误信息
	Error() error

	// ===  FUNCTION  ======================================================================
	//         Name:  Before
	//  Description:  step前置操作
	//                参数：input - 输入参数
	//                      prams - 全局参数
	//                返回值： 如果error不为nil，则跳过当前step，并将error作为input传递给下一个step
	// =====================================================================================
	Before(input interface{}, params ...interface{}) error

	// ===  FUNCTION  ======================================================================
	//         Name:  DoStep
	//  Description:  step核心过程
	//                参数：input - 输入参数
	//                      prams - 全局参数
	//                返回值：interface{} - 当前step的输出信息，会传递给下一个step作为input
	//                        error - step的错误信息，如果error不为nil，则会导致整个workflow终止
	// =====================================================================================
	DoStep(input interface{}, params ...interface{}) (interface{}, error)

	// ===  FUNCTION  ======================================================================
	//         Name:  After
	//  Description:  step后置操作
	//                参数：input - 输入参数
	//                      prams - 全局参数
	//                返回值： 如果error不为nil，并将error作为input传递给下一个step
	// =====================================================================================
	After(input interface{}, params ...interface{}) error
}

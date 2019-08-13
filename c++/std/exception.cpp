/*
// =====================================================================================
// 
//       Filename:  exception.cpp
// 
//    Description:  对C++中的标准异常进行总结
// 
//        Version:  1.0
//        Created:  02/24/2014 08:31:06 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <iostream>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <bits/ios_base.h>

void foo() throw(int)
{
	throw 1.1;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	try {
	} catch (std::bad_alloc &e) {		// inherit from exception
		// new操作失败时，会抛出bad_alloc异常（new的nothrow版本另当别论）
		std::cout << "bad_alloc" << std::endl;
	} catch (std::bad_cast &e) {		// inherit from exception
		// 执行期间，当一个作用于reference身上的“动态型别转换操作”失败时，dynamic_cast会抛出bad_cast异常
		std::cout << "bad_cast" << std::endl;
	} catch (std::bad_typeid &e) {		// inherit from exception
		// 执行期型别辨识（RTTI）过程中，如果交给typeid的参数为零或空指针，typeid操作符会抛出bad_typeid异常
		std::cout << "bad_typeid" << std::endl;

	} catch (std::domain_error &e) {	// inherit from logic_error
		// 指出专业领域范畴内的错误
		std::cout << "domain_error" << std::endl;
	} catch (std::invalid_argument &e) {	// inherit from logic_error
		// 表示无效参数，例如将bitset(array of bits)以char而非“0”或“1”进行初始化
		std::cout << "invalid_argument" << std::endl;
	} catch (std::length_error &e) {	// inherit from logic_error
		// 指出某个行为“可能超越了最大极限”，例如对某个字符串附加太多字符
		std::cout << "length_error" << std::endl;
	} catch (std::out_of_range &e) {	// inherit from logic_error
		// 指出参数值“不在预期范围内”，例如在处理容器或string中采用一个错误索引
		std::cout << "out_of_range" << std::endl;
	} catch (std::logic_error &e) {		// inherit from exception
		// c++标准程序库异常总是派生自logic_error
		std::cout << "logic_error" << std::endl;

	} catch (std::range_error &e) {		// inherit from runtime_error
		// 指出内部计算时发生区间错误
		std::cout << "range_error" << std::endl;
	} catch (std::overflow_error &e) {	// inherit from runtime_error
		// 指出算术运算发生上溢位
		std::cout << "overflow_error" << std::endl;
	} catch (std::underflow_error &e) {	// inherit from runtime_error
		// 指出算术运算发生下溢位
		std::cout << "underflow_error" << std::endl;
	} catch (std::runtime_error &e) {	// inherit from exception
		// 用来指出“不在程序范围内，且不容易回避”的事件
		std::cout << "runtime_error" << std::endl;

	} catch (std::ios_base::failure &e) {	// inherit from exception
		// 当数据流由于错误或者到达文件末尾而发生状态改变时，就可能抛出这个异常
		std::cout << "failure" << std::endl;

	} catch (std::bad_exception &e) {	// inherit from exception
		// 如果发生非预期的异常（函数抛出异常规格（exception specification）以外的异常），bad_exception异常会接手处理，bad_exception会调用unexpected()，后者通常会唤起terminate()终止程序
		// 如果异常规格罗列了bad_exception，那么任何未列于规格的异常，都将在函数unexpected()中重新抛出（rethrows）bad_exception
		e.what();	// virtual const char* what() const throw();
		std::cout << "bad_exception" << std::endl;

	} catch (std::exception &e) {		// base exception class
		e.what();	// virtual const char* what() const throw();
		std::cout << "exception" << std::endl;

	} catch (...) {
		std::cout << "..." << std::endl;
	}

	return 0;
}				// ----------  end of function main  ----------


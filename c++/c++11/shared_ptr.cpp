/*
// =====================================================================================
// 
//       Filename:  shared_ptr.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/2016 07:12:20 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <cstdlib>
#include <iostream>
#include <memory>

void del_fun(int *p)
{
	std::cout << "remove point array" << std::endl;
	delete[] p;
}

int ret_int()
{
	return 0;
}

void fun(int a, int b)
{
}

std::shared_ptr<int> ret(int value)
{
	return std::make_shared<int>(value);
}

struct A;
struct B;

struct A
{
	std::shared_ptr<B> bptr;
	~A() {std::cout << "A is deleted" << std::endl;}
};

struct B
{
	std::shared_ptr<A> aptr;
	~B() {std::cout << "B is deleted" << std::endl;}
};

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// shared_ptr的四种初始化方式
	// shared_ptr禁止直接使用new来进行赋值
	// std::shared_ptr<int> p = new int(1) // error
	std::shared_ptr<int> p1(new int(1));
	std::shared_ptr<int> p2 = p1;
	std::shared_ptr<int> p3;
	p3.reset(new int(1));
	std::shared_ptr<int> p4 = std::make_shared<int>(1);

	// shared_ptr的判断空的方式
	if (p1) {
		std::cout << "p1 is not null" << std::endl;
	}

	// 通过get方式获取裸指针
	int *ip = p1.get();

	// 自定义删除方式，只有当引用计数为0时，才会调用删除函数
	// 当删除数组时，必须手动指定删除函数，shared_ptr的默认删除，不支持删除数组
	std::shared_ptr<int> p5(new int[9], del_fun);
	std::shared_ptr<int> p6(p5);

	/* 使用智能指针需要注意的问题 */
	// 1、不要使用一个裸指针初始化多个shared_ptr
	int *i1 = new int(1);
	std::shared_ptr<int> p7(i1);
	std::shared_ptr<int> p8(i1);	// 造成重复delete

	// 2、不要在实参中创建shared_ptr
	fun(*std::shared_ptr<int>(new int(1)), ret_int());	// ret_int()调用失败，有可能造成内存泄漏

	// 3、不要使用shared_ptr返回this指针，容易发生重复delete

	// 4、避免shared_ptr指针的循环引用
	std::shared_ptr<A> ap(new A);
	std::shared_ptr<B> bp(new B);
	ap->bptr = bp;
	bp->aptr = ap;

	// shared_ptr无法支持c++动态数组指针，而unique_ptr可以
	// std::shared_ptr<int[]> parray(new int[4]);	// compile error
	std::shared_ptr<int> parray(new int[4]);	// 编译正确，但shared_ptr会调用delete去释放数组

	// shared_ptr作为返回值，必须有外部变量接收，否则会导致裸指针访问非法地址
	std::shared_ptr<int> pr = ret(4);	// ok
	int *pr1 = pr.get();	// ok
	int *pr2 = ret(5).get();	// point to an invalid address
	std::cout << "return is " << *pr << std::endl;	// read ok
	std::cout << "return is " << *pr1 << std::endl;	// read ok
	std::cout << "return is " << *pr2 << std::endl;	// invalid read

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


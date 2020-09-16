/*
// =====================================================================================
// 
//       Filename:  has_ptr.cpp
// 
//    Description:  本程序不涉及任何智能指针操作，仅仅就是为了说明普通指针存在的问题
// 
//        Version:  1.0
//        Created:  10/09/2012 11:25:30 AM
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

/*
// =====================================================================================
//        Class:  Has_Ptr
//  Description:  一个简单的类，用来展示普通指针存在的问题
// =====================================================================================
*/
class Has_Ptr {
public:
	// ====================  LIFECYCLE     =======================================
	Has_Ptr(int *p, int i) : ptr(p), val(i) {}	// constructor

	// ====================  ACCESSORS     =======================================
	int* get_ptr() const { return ptr; }
	int get_int() const { return val; }

	void set_ptr(int *p) { ptr = p; }
	void set_int(int i) { val = i; }

	int get_ptr_val() const { return *ptr; }
	void set_ptr_val(int val) const { *ptr = val;}	// 这里虽然是const函数，但是依然不妨碍修改指针所指向的值

private:
	// ====================  DATA MEMBERS  =======================================
	int *ptr;
	int val;

};	// -----  end of class Has_Ptr  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	int obj = 0;
	Has_Ptr ptr1(&obj, 42);
	Has_Ptr ptr2(ptr1);	// 编译器自动构造一个默认拷贝构造函数（浅拷贝）
	/* 复制之后，ptr1和ptr2中的指针指向同一个对象，且两个对象的int值相同
	 * 但是由于浅拷贝的原因，两个对象的int值是独立的，而指针则纠缠在一起*/

	/* 具有指针成员且使用默认拷贝构造函数的类，具有不同指针的所有缺陷
	 * 尤其是，类本身无法避免悬垂指针*/

	/* -------------------------------------------------------------- */

	ptr1.set_int(4);
	std::cout << ptr1.get_int() << std::endl;
	std::cout << ptr2.get_int() << std::endl;

	ptr1.set_ptr_val(5);
	std::cout << ptr1.get_ptr_val() << std::endl;
	std::cout << ptr2.get_ptr_val() << std::endl;
	/* 对于值修改，两个对象是相互独立的
	 * 但是，企图使用指针修改值，将会导致错误，一个指针的赋值操作将导致另一个对象的值也跟着改变*/

	/* -------------------------------------------------------------- */
	int *ip = new int(42);
	Has_Ptr ptr(ip, 10);
	delete ip;
	ptr.set_ptr_val(0);	// 此时指针所指向的地址已经被销毁（即使可能不会立即引起运行时错误）
	std::cout << ptr.get_ptr_val() << std::endl;
	/* 创建对象时，ptr对象中的指针与ip指向同一个地址空间，但是当ip释放时，ptr无法得知地址已经失效，造成悬垂指针的隐患 */

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


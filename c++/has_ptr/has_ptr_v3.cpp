/*
// =====================================================================================
// 
//       Filename:  has_ptr.cpp
// 
//    Description:  使用深拷贝的问题解决指针复制的问题
// 
//        Version:  1.0
//        Created:  10/10/2012 02:17:39 PM
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
//  Description:  在指针拷贝的过程中，使用深拷贝的方式进行复制
// =====================================================================================
*/
class Has_Ptr {
public:
	// ====================  LIFECYCLE     =======================================
	Has_Ptr(const int &p, int i) : ptr(new int(p)), val(i) {}	// constructor
	Has_Ptr(const Has_Ptr &orig) : 
		ptr(new int(*orig.ptr)), val(orig.val) {}
	~Has_Ptr() { delete ptr; }

	// ====================  INTERFACE     =======================================

	// ====================  ACCESSORS     =======================================
	int get_ptr_val() const { return *ptr; }
	int get_int() const { return val; }

	void set_ptr(int *p) { ptr = p; }
	void set_int(int i) { val = i; }

	int* get_ptr() const { return ptr; }
	void set_ptr_val(int i) const { *ptr = i; }

	// ====================  MUTATORS      =======================================

	// ====================  OPERATORS     =======================================

	Has_Ptr& operator=(const Has_Ptr &orig);

protected:
	// ====================  DATA MEMBERS  =======================================

private:
	// ====================  DATA MEMBERS  =======================================
	int *ptr;
	int val;

};	// -----  end of class Has_Ptr  -----

Has_Ptr& Has_Ptr::operator=(const Has_Ptr &rhs)
{
	*ptr = *rhs.ptr;
	val = rhs.val;
	return *this;
}		// -----  end of function operator=  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	int i = 42;
	Has_Ptr obj1(i, 10);
	std::cout << obj1.get_int() << ',' << obj1.get_ptr_val() << std::endl;
	std::cout << std::endl;

	Has_Ptr obj2(obj1);
	obj2.set_int(12);
	obj2.set_ptr_val(44);
	std::cout << obj1.get_int() << ',' << obj1.get_ptr_val() << std::endl;
	std::cout << obj2.get_int() << ',' << obj2.get_ptr_val() << std::endl;
	std::cout << std::endl;

	Has_Ptr obj3 = obj1;
	obj3.set_int(14);
	obj3.set_ptr_val(46);
	std::cout << obj1.get_int() << ',' << obj1.get_ptr_val() << std::endl;
	std::cout << obj3.get_int() << ',' << obj3.get_ptr_val() << std::endl;
	std::cout << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


/*
// =====================================================================================
// 
//       Filename:  has_ptr.cpp
// 
//    Description:  使用引用计数的方式实现智能指针，智能指针无需手动销毁
// 
//        Version:  1.0
//        Created:  10/10/2012 10:10:45 AM
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
//        Class:  U_Ptr
//  Description:  
// =====================================================================================
*/
class U_Ptr {
public:
	// ====================  LIFECYCLE     =======================================
	U_Ptr(int *p) : ip(p), use(1) {}	// constructor
	~U_Ptr() { delete ip; }

private:
	// ====================  DATA MEMBERS  =======================================
	friend class Has_Ptr;	// 友元不需要提前声明，可以直接使用名字
	int *ip;
	size_t use;

};	// -----  end of class U_Ptr  -----

/*
// =====================================================================================
//        Class:  Has_Ptr
//  Description:  通过U_Ptr类的辅助，实现智能指针
// =====================================================================================
*/
class Has_Ptr {
public:
	// ====================  LIFECYCLE     =======================================
	Has_Ptr(int *p, int i) : ptr(new U_Ptr(p)), val(i) {}	// constructor
	Has_Ptr(const Has_Ptr &orig) : 
		ptr(orig.ptr), val(orig.val) { ++ptr->use; }
	~Has_Ptr() { if (--ptr->use == 0) delete ptr; }

	// ====================  ACCESSORS     =======================================
	int *get_ptr() const { return ptr->ip; }
	int get_int() const { return val; }

	void set_ptr(int *p) { ptr->ip = p; }
	void set_int(int i) { val = i; }

	int get_ptr_val() const { return *ptr->ip; }
	void set_ptr_val(int i) { *ptr->ip = i; }
	
	size_t get_use() { return ptr->use; };	// 查看引用计数器

	// ====================  OPERATORS     =======================================
	Has_Ptr& operator=(const Has_Ptr&);

private:
	// ====================  DATA MEMBERS  =======================================
	U_Ptr *ptr;
	int val;

};	// -----  end of class Has_Ptr  -----

Has_Ptr& Has_Ptr::operator=(const Has_Ptr &rhs)
{
	++rhs.ptr->use;
	if (--ptr->use == 0) delete ptr;	// 左值需要酌情释放
	ptr = rhs.ptr;
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
	int *p = new int(42);
	Has_Ptr obj(p, 10);	// obj自己的val为10，指针指向的空间为42
	std::cout << obj.get_int() << ',' << obj.get_ptr_val() << ',' << obj.get_use() << std::endl;
	std::cout << std::endl;

	Has_Ptr obj2(obj);	// obj2复制obj
	std::cout << obj.get_int() << ',' << obj.get_ptr_val() << ',' << obj.get_use() << std::endl;
	std::cout << obj2.get_int() << ',' << obj2.get_ptr_val() << ',' << obj2.get_use() << std::endl;
	std::cout << std::endl;

	Has_Ptr obj3 = obj;	// obj3复制obj
	std::cout << obj.get_int() << ',' << obj.get_ptr_val() << ',' << obj.get_use() << std::endl;
	std::cout << obj2.get_int() << ',' << obj2.get_ptr_val() << ',' << obj2.get_use() << std::endl;
	std::cout << obj3.get_int() << ',' << obj3.get_ptr_val() << ',' << obj3.get_use() << std::endl;
	std::cout << std::endl;

	/* 下面这段代码将导致p所指向的地址重复释放 */
	Has_Ptr obj4(p, 4);	// obj自己的val为10，指针指向的空间为42
	std::cout << obj4.get_int() << ',' << obj4.get_ptr_val() << ',' << obj4.get_use() << std::endl;
	std::cout << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


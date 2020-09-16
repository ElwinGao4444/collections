/*
// =====================================================================================
// 
//       Filename:  demo.cpp
// 
//    Description:  介绍auto_ptr的使用方法，及其缺陷
//    		auto_ptr所使用的技术叫做RAII，即利用栈自动回收的特性回收地址空间
//    		auto_ptr的出现，主要是为了解决“被异常抛出时发生资源泄漏”的问题
//    		参考：http://www.cppblog.com/SmartPtr/archive/2007/07/05/27549.html
//    		auto_ptr源代码：http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-api-4.5/a00745_source.html
// 
//        Version:  1.0
//        Created:  10/10/2012 05:26:11 PM
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

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	/* 首先来看一下auto_ptr的用法 */
	int *pointer1 = new int(0);
	int *pointer2 = new int(1);
	std::auto_ptr<int> ap(pointer1);
	std::cout << *pointer1 << ',' << *ap << std::endl;
	std::cout << pointer1 << ',' << ap.get() << std::endl;
//	ap.release();	// release会使得auto_ptr内部指针置为NULL，但却不会释放地址空间，这里如果不注释掉，一定会造成内存泄漏
//	std::cout << pointer1 << ',' << ap.get() << std::endl;
	ap.reset(pointer2);	// reset会先释放之前的地址空间（如果不为NULL），再申请新的地址空间，而不是赋值
	std::cout << pointer2 << ',' << ap.get() << std::endl;
	std::cout << *pointer2 << ',' << *ap << std::endl;
	/* 
	 * 另外，auto_ptr还支持'*'、'='和'->'三种操作，其含义和普通指针类似 
	 * 其中'*'和'->'操作相对简单，没什么需要特别注意的地方
	 * 而'='操作需要特别注意：‘=’会使用release()，而非reset()，使用不当将会造成内存泄漏 
	 */
	return EXIT_SUCCESS;	// 程序到此结束

	/* 接下来看看auto_ptr存在的问题 */

	/* 首先，auto_ptr指针不能指向存储在heap以外的变量 */
	int i = 10;
	std::auto_ptr<int> ap1(&i);	// auto_ptr不能释放stack上的变量

	/* 
	 * 第二需要注意，由于auto_ptr在析构的时候，一定会无条件释放地址空间
	 * 所以，不允许两个auto_ptr指向同一个地址，否则会发生重复释放的问题
	 */
	int *p = new int(0);
	std::auto_ptr<int> ap2(p);
	std::auto_ptr<int> ap3(p);

	/* 第三，由于auto_ptr在析构中使用的是delete而不是delete[]，所以不能用auto_ptr来管理数组指针 */
	int *pa = new int[10];
	std::auto_ptr<int> ap4(pa);

	/* 由于auto_ptr的构造函数使用了explicit关键字，所以一切“裸”指针向auto_ptr的隐式转换都将被阻止 */
	/* 因为C++保证了删除一个空指针是安全的，所以auto_ptr在析构时，没有必要判断指针是否为NULL */

	/* 
	 * auto_ptr的拷贝过程，其实是移交过程，原变量将失去对指针的控制变为NULL 
	 * 这一特征的危险性体现于参数传递过程中，因为这种拷贝太过于隐蔽了
	 */

	/* 由于auto_ptr没有值语义，所以不能被用于stl标准容器中 */

	/* 
	 * 总结：
	 * 虽然我们上面介绍的还不够全面，但是也能看出auto_ptr的使用需要非常的谨慎
	 * 这种代价与直接使用裸指针不相上下，所以在一般的使用中，auto_ptr很少被用到
	 * 更多的是使用boost(tr1)库中的shared_ptr 
	 */
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

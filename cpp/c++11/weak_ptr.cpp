/*
// =====================================================================================
// 
//       Filename:  weak_ptr.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/2016 08:08:07 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <cstdlib>
#include <ios>
#include <iostream>
#include <memory>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// weak_ptr可以看作是shared_ptr的监视器
	std::shared_ptr<int> sp(new int(10));
	std::weak_ptr<int> wp(sp);

	std::cout << wp.use_count() << std::endl;

	// weak_ptr转shared_ptr
	std::shared_ptr<int> sp2 = wp.lock();

	std::cout << std::boolalpha << wp.expired() << std::endl;
	sp.reset();
	sp2.reset();
	std::cout << std::boolalpha << wp.expired() << std::endl;


	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


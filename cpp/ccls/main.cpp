/*
// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/11/2025 16:14:07
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

#include "a.hpp"
#include "b.hpp"
#include "c.hpp"

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::cout << A::inc(1) << std::endl;
	std::cout << B::inc(1) << std::endl;
	std::cout << C::inc(1) << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


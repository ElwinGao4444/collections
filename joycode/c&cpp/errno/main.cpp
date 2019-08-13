/*
// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/27/2015 02:46:35 PM
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

#include "errno.h"

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::cout << SUCCESS << ": " << errmsg(SUCCESS) << std::endl;
	std::cout << FAILURE << ": " << errmsg(FAILURE) << std::endl;

	std::cout << SUCCESS << ": " << errmsg(SUCCESS, "[%s]", "diy") << std::endl;
	std::cout << FAILURE << ": " << errmsg(FAILURE, "[%s]", "diy") << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


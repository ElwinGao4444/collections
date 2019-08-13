/*
// =====================================================================================
// 
//       Filename:  derived.hpp
// 
//    Description:  派生类声明
// 
//        Version:  1.0
//        Created:  01/16/2013 01:07:46 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include "base.hpp"

#ifndef  DERIVED_HPP
#define  DERIVED_HPP

// =====================================================================================
//        Class:  Derived
//  Description:  
// =====================================================================================
class Derived : public Base
{
public:
	// ====================  LIFECYCLE     =======================================
	Derived(){}		// constructor

	// ====================  INTERFACE     =======================================
	int derived_fun_1();
	int derived_fun_2();
	int derived_fun_3();

private:
	// ====================  DATA MEMBERS  =======================================

};		// -----  end of class Derived  -----

#endif  /*DERIVED_HPP*/

/*
// =====================================================================================
// 
//       Filename:  RTTI.cpp
// 
//    Description:  Run-Time Type Identification，运行时类型识别
// 
//        Version:  1.0
//        Created:  03/30/2016 07:01:56 PM
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
#include <typeinfo>

class Base {
public:
	virtual ~Base() {}
};

class Male : public Base {
};

class Female : public Base {
};

Base* create(char sign)
{
	if (sign == 'm') {
		return new Male();
	} else if (sign == 'f') {
		return new Female();
	} else {
		return new Base();
	}
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	char sign = argv[1][0];
	Base *p = create(sign);
	std::cout << typeid(p).name() << std::endl;
	std::cout << typeid(*p).name() << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

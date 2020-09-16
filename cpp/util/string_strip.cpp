/*
// =====================================================================================
// 
//       Filename:  string_strip.cpp
// 
//    Description:  去掉字符串前后的空白符
// 
//        Version:  1.0
//        Created:  12/29/2016 04:57:25 PM
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

#include <string>
#include <typeinfo>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  lstrip
//  Description:  去掉左空白符
// =====================================================================================
*/
std::string lstrip(const std::string &str)
{
	for (size_t i = 0; i < str.length(); ++i) {
		if (!isspace(str[i])) {
			return str.substr(i);
		}
	}
	return std::string();
}		// -----  end of function lstrip  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  rstrip
//  Description:  去掉左空白符
// =====================================================================================
*/
std::string rstrip(const std::string &str)
{
	for (size_t i = str.length() - 1; i > 0; --i) {
		if (!isspace(str[i])) {
			return str.substr(0, i + 1);
		}
	}
	if (isspace(str[0])) {
		return std::string();
	} else {
		return std::string(1, str[0]);
	}
}		// -----  end of function rstrip  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  strip
//  Description:  去掉两端空白符
// =====================================================================================
*/
std::string strip(const std::string &str)
{
	size_t lspace = 0;
	size_t rspace = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		if (!isspace(str[i])) {
			lspace = i;
			break;
		}
	}
	for (size_t i = str.length() - 1; i > 0; --i) {
		if (!isspace(str[i])) {
			rspace = i;
			break;
		}
	}
	if (lspace < rspace) {
		return str.substr(lspace, rspace - lspace + 1);
	} else if (lspace  == rspace && !isspace(str[lspace])) {
		return std::string(1, str[lspace]);
	} else {
		return std::string();
	}
}		// -----  end of function strip  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::string str1 = "  this is a test string.  ";
	std::string str2 = "\t\n this is a another test string. \n\t";

	std::string str;
	str = str1.substr(str1.find_first_not_of(' '));
	std::cout << "|" << str << "|" << std::endl;
	str = str1.substr(0, str1.find_last_not_of(' ') + 1);
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	str = str1.erase(0, str1.find_first_not_of(' '));
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "|" << str1 << "|" << std::endl;
	str = str1.erase(str1.find_last_not_of(' ') + 1);
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "|" << str1 << "|" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	str = lstrip(str2);
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("x");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip(" ");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("  x  ");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("     ");
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	str = rstrip(str2);
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("x");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip(" ");
	std::cout << "|" << str << "|" << std::endl;
	str = rstrip("  x  ");
	std::cout << "|" << str << "|" << std::endl;
	str = rstrip("     ");
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	str = strip(str2);
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip("x");
	std::cout << "|" << str << "|" << std::endl;
	str = lstrip(" ");
	std::cout << "|" << str << "|" << std::endl;
	str = strip("  x  ");
	std::cout << "|" << str << "|" << std::endl;
	str = strip("     ");
	std::cout << "|" << str << "|" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	std::cout << typeid(str[0]).name() << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


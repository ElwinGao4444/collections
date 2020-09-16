/*
// =====================================================================================
// 
//       Filename:  tuple.cpp
// 
//    Description:  tuple用法小结
// 
//        Version:  1.0
//        Created:  12/29/2016 07:53:15 PM
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

#include <tuple>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	int i = 0;
	double d = 0.0;
	std::string s = "0";
	std::tuple<int, double, std::string> tp;

	// tuple_size可以用于获取tuple的大小
	std::cout << std::tuple_size<decltype(tp)>::value << std::endl;

	// make_tuple只接受右值引用，而不能使用左值引用
	i = 0; d = 0.0; s = "0";
	tp = std::make_tuple<int, double, std::string>(std::move(i), std::move(d), std::move(s));
	std::cout << "<" << std::get<0>(tp) << ", " << std::get<1>(tp) << ", " << std::get<2>(tp) << ">" << std::endl;
	// forward_as_tuple和make_tuple类似，但区别在于forward_as_tuple返回的结果是右值引用，而make_tuple返回的结果是对象
	// 以上只是文档上描述的区别，在实际使用中，并未发现二者有什么太大区别
	tp = std::forward_as_tuple<int, double, std::string>(std::move(i), std::move(d), std::move(s));
	std::cout << "<" << std::get<0>(tp) << ", " << std::get<1>(tp) << ", " << std::get<2>(tp) << ">" << std::endl;

	// tie只接受左值引用，这一点是和make_tuple最大的不同
	i = 0; d = 0.0; s = "0";
  	tp = std::tie<int, double, std::string>(i, d, s);
  	std::cout << "<" << std::get<0>(tp) << ", " << std::get<1>(tp) << ", " << std::get<2>(tp) << ">" << std::endl;
	// tie也是赋值操作，而不是引用操作
  	i = 1; d = 1.1; s = "1";
  	std::cout << "<" << std::get<0>(tp) << ", " << std::get<1>(tp) << ", " << std::get<2>(tp) << ">" << std::endl;
	// tie还能反向获取tuple中全部变量的值
	// tie和make_tuple都可以不用指定变量类型，编译器可以自行推导
	std::tie(i, d, s) = std::make_tuple(2, 2.2, "2");
  	std::cout << "<" << i << ", " << d << ", " << s << ">" << std::endl;
	// 如果只关心某一个值，可以使用ignore来忽略其他值
	std::tie(std::ignore, d, std::ignore) = std::make_tuple(3, 3.3, "3");
  	std::cout << "<" << i << ", " << d << ", " << s << ">" << std::endl;

	// tuple_cat用于拼接tuple，这是tuple相比struct结构最大的优势，即可以动态增减字段
	// tuple和pair可以混用，二者只是二元特化和多元泛化的区别，本质相同
	auto tp_cat = std::tuple_cat(tp, std::make_pair("foo", "bar"), std::tie(d));
	std::cout << std::tuple_size<decltype(tp_cat)>::value << std::endl;

	// tuple_element可以动态获知tuple指定位置的数据类型，这对于动态增减tuple大小时非常有用
	std::tuple_element<0, decltype(tp_cat)>::type first = std::get<0>(tp_cat);
  	std::cout << "tpcat[0] = " << first << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


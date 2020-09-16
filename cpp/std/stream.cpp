/*
// =====================================================================================
// 
//       Filename:  iostream.cpp
// 
//    Description:  IO流
//
//                                    ios_base
//                                       ^
//                                       |
//                                      ios
//                                       ^
//                                       |
//                       ---------------------------------
//                       ^                               ^
//                       |                               |
//                    istream                         ostream
//                    ( cin )                    (cout, cerr, clog)
//                       |                               |
//        ------------------------------- -------------------------------
//        ^              ^              ^ ^              ^              ^
//        |              |              | |              |              |
//  istringstream    ifstream         iostream        ofstream    ostringstream
//                                       ^
//                                       |
//                       ---------------------------------
//                       ^                               ^
//                       |                               |
//                 istringstream                      fstream
//
//                                   streambuf
//                                       ^
//                                       |
//                       ---------------------------------
//                       ^                               ^
//                       |                               |
//                   stringbuf                        filebuf
// 
//        Version:  1.0
//        Created:  09/12/2015 05:47:25 PM
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
#include <fstream>
#include <string>

// 基本文件输入输出
int fstream()
{
/* 
// =====================================================================================
//      打开文件的方式在ios类(所以流式I/O的基类)中定义，有如下几种方式：
//      ios::           为输入(读)而打开文件
//      ios::out        为输出(写)而打开文件
//      ios::ate        初始位置：文件尾
//      ios::app        所有输出附加在文件末尾
//      ios::trunc      如果文件已存在则先删除该文件
//      ios::binary     二进制方式"
// =====================================================================================
*/
	// 基本文件读写
	std::ifstream readf;
	std::ofstream writef;
	writef.open("fstream.dat", std::ios::out);	// 第二个参数可选，ofstream默认模式为ios::out
	if (writef.is_open()) {
		writef << "first line" << std::endl;
		writef << "second line" << std::endl;
		writef << "third line" << std::endl;
		writef.put('a');
		writef.put('b');
		writef.put('c');
		writef.put('d');
		writef.close();	// 即使不close，析构的时候也会自动close
	}
	readf.open("fstream.dat", std::ios::in);	// 第二个参数可选，ifstream默认模式为ios::in
	if (readf.is_open()) {
		char buf[1024];
		std::string str;

		// getline方法
		readf.getline(buf, 1024);	// getline还可以通过第三个参数指定结束符
		std::cout << buf << std::endl;

		// 读取单个字符
		std::cout << static_cast<char>(readf.get());	// 获取单个字符
		std::cout << static_cast<char>(readf.peek());	// 获取单个字符，但文件指针不后移
		std::cout << static_cast<char>(readf.get());
		std::cout << static_cast<char>(readf.get());
		std::cout << static_cast<char>(readf.get());
		std::cout << static_cast<char>(readf.get());
		std::cout << static_cast<char>(readf.get());

		readf.unget();	// 将刚取出的char再放回流中
		readf.unget();	// 将刚取出的char再放回流中
		std::cout << static_cast<char>(readf.get());
		std::cout << static_cast<char>(readf.get());

		readf.putback(';');	// 将指定字符放回流中
		std::cout << static_cast<char>(readf.get());

		// 输入流方法
		readf >> str;	// 读到空白符为止
		std::cout << str << std::endl;

		// 处理文件位置
		std::cout << readf.tellg() << std::endl;	// 获取当前文件指针的offset
		readf.seekg(0);	// 设置文件指针的offset

		// 按行读取
		for ( ; ; ) {
			readf.getline(buf, 1024);	// 最后一行没有回车则无法得到最后一行
			if (readf.eof()) break;	// 检查是否读到EOF
			std::cout << buf << std::endl;
		}

		readf.close();	// 即使不close，析构的时候也会自动close
	}

/* 
// 不同方法对应的state说明
// =====================================================================================
// iostate value
// (member constants)   indicates                               functions to check state flags
//                                                              good()  eof()   fail()  bad()   rdstate()
// goodbit              No errors (zero value iostate)          true    false   false   false   goodbit
// eofbit               End-of-File reached on input operation  false   true    false   false   eofbit
// failbit              Logical error on i/o operation          false   false   true    false   failbit
// badbit               Read/writing error on i/o operation     false   false   true    true    badbit
// =====================================================================================
*/
	std::fstream file("/no/such/file");	// 与直接调用open一样，fstream默认为ios::in | ios::out
	if (!file) {	// 流对象的值为0也可以用来表示打开失败
		std::cout << "file open error" << std::endl;
		file.get();
		if (!file.good()) {	// good方法属于广泛的检查上一个调用是否成功的函数
			std::cout << "file get error" << std::endl;
		}
	}

	return 0;
}

// 标准输入输出
int stdstream()
{
	std::string s;
	std::cout << "please input any string" << std::endl;
	std::cin >> s;
	std::cout << s << std::endl;

	return 0;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	stdstream();
	fstream();

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


/*
// =====================================================================================
// 
//       Filename:  foo.cpp
// 
//    Description:  split string by using std::getline()
// 
//        Version:  1.0
//        Created:  12/20/2016 11:27:23 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <iostream>
using std::cout; using std::getline;
#include <iterator>
using std::ostream_iterator;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <vector>
using std::vector;
#include <algorithm>
using std::copy;

vector<string> &split(const string &str, char delim, vector<string> &elems, bool skip_empty = true) {
	istringstream iss(str);
	for (string item; getline(iss, item, delim); ) {
		if (skip_empty && item.empty()) {
			continue;
		} else {
			elems.push_back(item);
		}
	}
	return elems;
}

int main()
{
	string str("Windows,Linux,,MacOS,");
	vector<string> result;
	split(str, ',', result);
	copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n"));
	return 0;
}


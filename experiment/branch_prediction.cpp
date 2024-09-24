/*
// =====================================================================================
// 
//       Filename:  branch_prediction.cpp
// 
//    Description:  通过一个例子，来证明CPU分支预测对程序带来的影响
// 
//        Version:  1.0
//        Created:  09/24/2024 17:41:40
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <ctime>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <algorithm>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	const int n = 1000*1000;
	int arr[n];
	int sum = 0;

	// 随机初始化数组
	std::srand(static_cast<unsigned int>(std::time(0)));
	for (int i = 0; i < n; ++i) {
		arr[i] = std::rand() % 256;
	}

	auto start = std::chrono::high_resolution_clock::now();
	// std::sort(arr, arr + n);	// 排序以后，程序的执行性能明显提高

	for (int i = 0; i < 100; i++) {
		for (int i = 0; i < n; i++) {
			if (arr[i] < 128) {	// 原因是，排序之前，if的分支预测准确率约为50%，会导致频繁清空流水线，但排序后，分支预测准确率接近100%
				sum += arr[i];
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "操作耗时: " << elapsed.count() << " 毫秒" << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


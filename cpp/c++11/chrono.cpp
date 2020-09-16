/*
// =====================================================================================
// 
//       Filename:  chrono.cpp
// 
//    Description:  duration的原型如下：
//                  template<
//                      class Rep,
//                      class Period = std::ratio<1, 1>
//                  > class duration;
//                  Rap是一个数值类型，表示时钟数的类型
//                  Period是一个默认模板参数std::ratio，表示时钟周期，原型如下：
//                  template<
//                      std::intmax_t Num,
//                      std::intmax_t Denom = 1,
//                  > class ratio;
//                  它表示每个时钟周期的秒数，Num代表分子，Denom代表分母，因此ratio表示
//                  的是一个分数数值，单位为秒
//
//                  time_point的原型如下：
//                  template <
//                      class Clock,
//                      class Duration = typename Clock::duration
//                  > class time_point;
//                  Clock是一个时钟类型，内部有time_point、duration、Rep、Period等信息，
//                  主要用于获取当前，时间以及实现time_t和time_point的相互转换。Clock包含
//                  以下3种时钟：
//                      system_clock：挂钟时间，依赖于系统时间设定
//                      steady_clock：严格递增的时间，不受系统时间影响
//                      high_resolution_clock：高精度时钟，属于system_clock和steady_clock的别名
//                  Duration就是上面说道的duration类型，用于记录时间间隔
// 
//        Version:  1.0
//        Created:  06/20/2016 03:10:08 PM
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
#include <chrono>

#include <unistd.h>


// =====================================================================================
//        Class:  Timer
//  Description:  用high_resolution_clock实现一个timer
// =====================================================================================
class Timer
{
public:
	// ====================  LIFECYCLE     =======================================
	Timer() : m_begin(std::chrono::high_resolution_clock::now()) {}
	void reset() { m_begin = std::chrono::high_resolution_clock::now(); }

	// 默认输出毫秒
	template<typename Duration = std::chrono::milliseconds>
		int64_t elapsed() const
		{
			return std::chrono::duration_cast<Duration>(std::chrono::high_resolution_clock::now() - m_begin).count();
		}


	// ====================  INTERFACE     =======================================
	// 纳秒
	int64_t elapsed_nano() const
	{
		return elapsed<std::chrono::nanoseconds>();
	}

	// 微秒
	int64_t elapsed_micro() const
	{
		return elapsed<std::chrono::microseconds>();
	}

	// 毫秒
	int64_t elapsed_milli() const
	{
		return elapsed<std::chrono::milliseconds>();
	}

private:
	// ==================== PRIVATE METHOD =======================================

	// ====================  DATA MEMBERS  =======================================
	std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;

};		// -----  end of class Timer  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// 定义一个时间间隔单位：30hz，并为其初始化3.5个单位
	std::chrono::duration<double, std::ratio<1, 30>> hz30{3.5}; // 大括号，小括号，都可以

	// 标准库中定义了一些常用的时间间隔：
	// typedef duration <Rap, ratio<3600, 1>> hours;
	// typedef duration <Rap, ratio<60, 1>> minutes;
	// typedef duration <Rap, ratio<1, 1>> seconds;
	// typedef duration <Rap, ratio<3600, 1>> hours;
	// typedef duration <Rap, ratio<1, 1000>> milliseconds;
	// typedef duration <Rap, ratio<1, 1000000>> microseconds;
	// typedef duration <Rap, ratio<1, 1000000000>> nanoseconds;
	std::chrono::minutes t1(10);
	std::chrono::seconds t2(60);
	// 数值之间可以直接做不同单位的时间可以直接做运算
	std::chrono::seconds t3 = t1 - t2;
	// 通过count方法获取间隔的差值
	std::cout << t3.count() << std::endl;

	// 将当前时间用记录天数的ratio来记录
	typedef std::chrono::duration<int, std::ratio<60*60*24>> days_type;
	std::chrono::time_point<std::chrono::system_clock, days_type> today = 
		std::chrono::time_point_cast<days_type>(std::chrono::system_clock::now());
	// time_since_epoch用于获取从1970年1月1日到time_point时间所经过的duration
	std::cout << today.time_since_epoch().count() << " days since epoch" << std::endl;

	// time_point可以直接与duration进行算数运算
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t last = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
	std::time_t next = std::chrono::system_clock::to_time_t(now + std::chrono::hours(24));
	char time_str[BUFSIZ];
	strftime(time_str, sizeof(time_str), "%Y-%m-%d,%H:%M:%S", localtime(&last));  
	std::cout << "yesterday is " << time_str << std::endl;
	strftime(time_str, sizeof(time_str), "%Y-%m-%d,%H:%M:%S", localtime(&next));  
	std::cout << "tommorow is " << time_str << std::endl;

	// time_point可以与另一个time_point进行算数运算
	std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
	std::chrono::nanoseconds diff = tp2 - tp1;  // system_clock内置的duration为nanoseconds类型
	std::cout << diff.count() << std::endl;

	// 自制定时器
	Timer timer;
	usleep(1000);
	std::cout << timer.elapsed_milli() << std::endl;
	std::cout << timer.elapsed_micro() << std::endl;
	std::cout << timer.elapsed_nano() << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


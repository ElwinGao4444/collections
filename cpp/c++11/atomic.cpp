/*
// =====================================================================================
// 
//       Filename:  atomic.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2017/10/03 13时27分04秒
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
#include <cassert>

#include <atomic>

// =====================================================================================
//        Class:  SpinLock
//  Description:  利用std::atomic_flag实现自旋锁
// =====================================================================================
class SpinLock
{
public:
	// ====================  LIFECYCLE     =======================================
	SpinLock() : flag(ATOMIC_FLAG_INIT) {};
	~SpinLock() {}

	// ====================  INTERFACE     =======================================
	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}

	void unlock()
	{
		flag.clear(std::memory_order_release);
	}

private:
	// ==================== PRIVATE METHOD =======================================

	// ====================  DATA MEMBERS  =======================================
	std::atomic_flag flag;

};		// -----  end of class SpinLock  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// 最简单的原子类型，只表示一个bool标记，但与atomic_bool(std::atomic<bool>)是不同的
	// std::atomic_flag对象必须被ATOMIC_FLAG_INIT初始化
	std::atomic_flag flag = ATOMIC_FLAG_INIT;		// init false
	// std::atomic_flag只支持clear()和test_and_set()操作
	flag.clear(std::memory_order_release);			// set flag to false;
	flag.test_and_set(std::memory_order_acquire);	// set flag to true and return false;
	flag.test_and_set(std::memory_order_acquire);	// set flag to true and return true;
	std::cout << "std::atomic_flag ok" << std::endl;

	// std::atomic_flag实现自旋锁
	SpinLock spinlock;
	spinlock.lock();
	spinlock.unlock();

	// 基本原子类型操作
	// 两个atomic变量之间，是不能拷贝构造与拷贝赋值的，因为这种操作破坏了原子性
	std::atomic<bool> b(false);
	bool x;
	x = b.load(std::memory_order_acquire);			// read and return
	std::cout << std::boolalpha << x << std::endl;
	b.store(true, std::memory_order_release);		// just write 
	x = b.exchange(false, std::memory_order_acq_rel);	// read and write
	std::cout << std::boolalpha << x << std::endl;
	// atomic的cas操作
	// 当原子变量与预期值相等时，修改原子变量为目标值，并返回true
	// 当原子变量与预期值不等时，修改预期值为原子变量的值，并返回false
	// compare_exchange_weak()在缺乏cas指令的机器上，可能会出现“伪失败”，
	// 即原子变量与预期值相等，但依旧赋值失败的现象，所以通常需要一个循环
	std::cout << "std::atomic<bool> cas" << std::endl;
	bool expected = false;
	while(!b.compare_exchange_weak(expected, true,
				std::memory_order_release,
				std::memory_order_relaxed) && !expected);
	std::cout << std::boolalpha << expected << std::endl;
	std::cout << std::boolalpha << b << std::endl;
	// compare_exchange_strong()与compare_exchange_weak()功能一致
	// 区别在于compare_exchange_strong不会出现“伪失败”问题
	// 所以compare_exchange_strong()不需要使用循环来进行保护
	b.compare_exchange_strong(expected, false);
	std::cout << std::boolalpha << expected << std::endl;
	std::cout << std::boolalpha << b << std::endl;
	b.compare_exchange_strong(expected, false);
	std::cout << std::boolalpha << expected << std::endl;
	std::cout << std::boolalpha << b << std::endl;
	std::cout << "std::atomic<bool> ok" << std::endl;

	// 6种内存序列
	// Relaxed ordering: memory_order_relaxed
	// 没有同步和顺序的制约，仅保证操作的原子性
	// Relaxed ordering常用语shared_ptr，因为指针引用的增减，并不依赖于内存序
	// 从CPU角度来看，memory_order_relaxed既不会影响乱序执行，也不会影响Cache一致
	{
		std::atomic<int> cnt = {0};
		cnt.fetch_add(1, std::memory_order_relaxed);
	}
	
	// Release-Consume ordering: memory_order_consume
	// 当前线程依赖于该原子变量值的读写操作，不能被重排到此操作之前
	// 其他线程对该原子变量的写入，在本线程可见
	// 从CPU角度来看，memory_order_consume要求原子变量必须从内存读取
	{
		// Release-Consume ordering只关心原子变量本身的内存序，所以不能当做guard使用
		std::atomic<std::string*> ptr;
		int data;
		// Thread Producer
		{
			std::string* p  = new std::string("Hello");
			data = 42;
			ptr.store(p, std::memory_order_release);
		}
		// Thread Consumer
		{
			std::string* p2;
			while (!(p2 = ptr.load(std::memory_order_consume)));
			assert(*p2 == "Hello");	// never fires: *p2 carries dependency from ptr
			assert(data == 42);	// may or may not fire: data does not carry dependency from ptr
		}
	}

	// Release-Acquire ordering: 
	// memory_order_acquire:
	// 当前线程中的读写操作不允许重排到该原子变量的acquire之前
	// 其他release同一原子变量的全部写操作，对当前线程可见
	// 从CPU角度来看，memory_order_acquire要求CPU清空读缓冲区，数据全部重新从内存加载
	// memory_order_release:
	// 当前线程中的读写操作不允许重排到该原子变量的release之前
	// 当前线程所有写入，对其他acquire该原子变量的线程可见
	// 当前线程对该原子变量的写入，对其他consume该原子变量的线程可见
	// 从CPU角度来看，memory_order_release要求写缓冲全部同步到内存
	// memory_order_acq_rel:
	// 同时具有memory_order_acquire和memory_order_release的特征
	{
		// Release-Consume ordering只关心原子变量本身的内存序，所以不能当做guard使用
		std::atomic<std::string*> ptr;
		int data;
		// Thread Producer
		{
			std::string* p  = new std::string("Hello");
			data = 42;
			ptr.store(p, std::memory_order_release);
		}
		// Thread Consumer
		{
			std::string* p2;
			while (!(p2 = ptr.load(std::memory_order_acquire)));
			assert(*p2 == "Hello");	// never fires
			assert(data == 42);	// never fires
		}
	}

	// Sequentially-consistent ordering: memory_order_seq_cst
	// memory_order_seq_cst是memory_order_acq_rel的强化版
	// memory_order_acq_rel只会对与当前修改的原子变量相关的线程产生作用
	// memory_order_seq_cst会对所有线程都产生作用，无论这个线程是否使用了该原子变量
	// 从CPU角度来看，其他memory_order_xxx只影响当前CPU，
	// 而memory_order_seq_cst会强迫所有CPU同步写缓冲，并清空读缓冲
	{
		// 最终结果: z != 0
		std::atomic<bool> x = {false};
		std::atomic<bool> y = {false};
		std::atomic<int> z = {0};
		// Thread write_x
		{
			x.store(true, std::memory_order_seq_cst);
		}
		// Thread write_y
		{
			y.store(true, std::memory_order_seq_cst);
		}
		// Thread read_x_then_y
		{
			while (!x.load(std::memory_order_seq_cst));
			if (y.load(std::memory_order_seq_cst)) {
				++z;
			}
		}
		// Thread read_y_then_x
		{
			while (!y.load(std::memory_order_seq_cst));
			if (x.load(std::memory_order_seq_cst)) {
				++z;
			}
		}
	}

	// 查看原子变量是否是lock free的
	struct A { int a[100]; };
	struct B { int x, y; };
	std::cout << std::boolalpha	// 将bool值显示为true or false
		<< "std::atomic<A> is lock free? "
		<< std::atomic<A>{}.is_lock_free() << '\n'
		<< "std::atomic<B> is lock free? "
		<< std::atomic<B>{}.is_lock_free() << '\n';

	std::cout << "Hello World" << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


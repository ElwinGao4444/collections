/*
// =====================================================================================
// 
//       Filename:  atomic_reference.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/23/2016 11:05:22 AM
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
#include <thread>
#include <mutex>

// =====================================================================================
//        Class:  AtomicReference
//  Description:  
// =====================================================================================

template <typename T>
class AtomicReference
{
public:
	// ====================  LIFECYCLE     =======================================
	AtomicReference() = default;

	AtomicReference(const AtomicReference &obj)
	{
		std::lock_guard<std::mutex> guard(_lock);
		_pptr = obj._pptr;
	}


	AtomicReference(T *ptr) : _pptr(new std::shared_ptr<T>(ptr))
	{
	}

	~AtomicReference()
	{
	}

	// ====================  INTERFACE     =======================================

	// ====================  OPERATORS     =======================================
	AtomicReference& operator=(const AtomicReference &obj)
	{
		std::lock_guard<std::mutex> guard(_lock);
		_pptr = obj._pptr;
		return *this;
	}

	AtomicReference& operator=(T *ptr)
	{
		std::lock_guard<std::mutex> guard(_lock);
		if (_pptr.get() == nullptr) {
			_pptr = std::shared_ptr<std::shared_ptr<T>> (new std::shared_ptr<T>(ptr));
		} else {
			*_pptr = std::move(std::shared_ptr<T>(ptr));
		}
		return *this;
	}

	std::shared_ptr<T> operator->()
	{
		std::lock_guard<std::mutex> guard(_lock);
		std::shared_ptr<T> tmp = *_pptr;
		return std::move(tmp);
	}

//	std::shared_ptr<T> operator*() {return *_pptr;}

private:
	// ==================== PRIVATE METHOD =======================================

	// ====================  DATA MEMBERS  =======================================
	std::shared_ptr<std::shared_ptr<T>> _pptr;
	std::mutex _lock;
};		// -----  end of template class AtomicReference  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::shared_ptr<std::shared_ptr<int>> p(new std::shared_ptr<int>(new int(3)));
	std::cout << **p << std::endl;

	AtomicReference<std::string> s0;
	s0 = new std::string("x");
	std::cout << s0->c_str() << std::endl;

	AtomicReference<std::string> s1(new std::string("abc"));
	AtomicReference<std::string> s2 = s1;
	std::cout << s1->c_str() << std::endl;
	std::cout << s2->c_str() << std::endl;

	s1 = new std::string("bcd");
	std::cout << s1->c_str() << std::endl;
	std::cout << s2->c_str() << std::endl;

	std::cout << "Hello World" << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


/*
// =====================================================================================
// 
//       Filename:  test_driver.cpp
// 
//    Description:  测试用例
// 
//        Version:  1.0
//        Created:  01/16/2013 12:59:28 PM
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

#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include "base.hpp"
#include "derived.hpp"

void log4cpp_demo()
{
	/* log4cpp有两种配置模式：1、代码配置模式。2、文件配置模式。代码配置模式可以帮助我们理解其运行原理，但是用起来非常不方便，所以一般都使用文件配置模式 */
	/* log4cpp的文件配置模式 */
	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

	log4cpp::Category& root = log4cpp::Category::getRoot();

	log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

	log4cpp::Category& sub2 = log4cpp::Category::getInstance(std::string("sub1.sub2"));

	root.warn("Storm is coming");

	sub1.debug("Received storm warning");
	sub1.info("Closing all hatches");

	sub2.debug("Hiding solar panels");
	sub2.error("Solar panels are blocked");
	sub2.debug("Applying protective shield");
	sub2.warn("Unfolding protective shield");
	sub2.info("Solar panels are shielded");

	sub1.info("All hatches closed");

	root.info("Ready for storm.");

	log4cpp::Category::shutdown();
}

// =====================================================================================
//        Class:  base_test
//  Description:  这个类是我们通常会用到的类，一般来说，我们用一个测试类对应一个功能类
// =====================================================================================
class base_test : public CppUnit::TestCase
{
public:
	void setUp()
	{
		std::cout << "base_test::setUp()" << std::endl;
	}

	void tearDown()
	{
		std::cout << "base_test::tearDown()" << std::endl;
	}

	void test_fun_1()
	{
		std::cout << "base_test::test_fun_1()" << std::endl;
		obj.base_fun_1();
	}

	void test_fun_2()
	{
		std::cout << "base_test::test_fun_2()" << std::endl;
		obj.base_fun_2();
	}

	void test_fun_3()
	{
		std::cout << "base_test::test_fun_3()" << std::endl;
		obj.base_fun_3();
	}

	CPPUNIT_TEST_SUITE(base_test);
	CPPUNIT_TEST(test_fun_1);
	CPPUNIT_TEST(test_fun_2);
	CPPUNIT_TEST(test_fun_3);
	CPPUNIT_TEST_SUITE_END();

private:
	Base obj;
};

// =====================================================================================
//        Class:  derived_test
//  Description:  这个类一般用到的很少，只有在多人增量式开发时才会用到，derived_test一般
//                用来编写一些在base_test之上的增量测试
//                注意：derived_test并不是说对派生类进行测试需要使用这个类，实际上，对任
//                何功能类的测试，无论这个功能类是基类还是派生类，都应该使用base_test模型
// =====================================================================================
class derived_test : public base_test
{
public:
	void setUp()
	{
		std::cout << "derived_test::setUp()" << std::endl;
	}

	void tearDown()
	{
		std::cout << "derived_test::tearDown()" << std::endl;
	}

	void test_fun_1()
	{
		std::cout << "derived_test::test_fun_1()" << std::endl;
		obj.base_fun_1();
	}

	void test_fun_2()
	{
		std::cout << "derived_test::test_fun_2()" << std::endl;
		obj.base_fun_2();
	}

	void test_fun_3()
	{
		std::cout << "derived_test::test_fun_3()" << std::endl;
		obj.base_fun_3();
	}

	CPPUNIT_TEST_SUB_SUITE(derived_test, base_test);
	CPPUNIT_TEST(test_fun_1);
	CPPUNIT_TEST(test_fun_2);
	CPPUNIT_TEST(test_fun_3);
	CPPUNIT_TEST_SUITE_END();

private:
	Derived obj;
};

CPPUNIT_TEST_SUITE_REGISTRATION(base_test);	// 针对base_test模型的测试集合注册，一般对多个功能类进行测试，这里会注册多个base_test
CPPUNIT_TEST_SUITE_REGISTRATION(derived_test);	// 针对derived_test模型的测试集合，如果注册derived_test就不必再注册base_test，cppunit会自动的执行基类测试用例
/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	/* 这部分代码一般是固定的，测试用例的多少是由CPPUNIT_TEST_SUITE_REGISTRATION宏决定的 */
	/* 通过执行代码，可以发现base_test的3个测试用例被测试了两遍，这也就是为什么当我们打算使用derived_test模型进行测试的时候，就不应该再注册base_test模型的原因 */
	CppUnit::Test *test = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextTestRunner runner;
	runner.addTest(test);
	runner.run();

	/* log4cpp用法实例 */
	log4cpp_demo();

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

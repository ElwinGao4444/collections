/*
 * =====================================================================================
 *
 *       Filename:  test_driver.c
 *
 *    Description:  测试驱动
 *
 *        Version:  1.0
 *        Created:  01/10/2013 01:13:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
/* #include <CUnit/Automated.h> */

#include <log4c.h>

#include "demo.h"
#include "demo_test.h"

/* 介绍log4c的用法 */
static void log4c_demo()
{
	log4c_category_t* runlog = NULL;

	if (log4c_init()){
		printf("log4c_init() failed");
		return;
	}

	runlog = log4c_category_get("runlog");
	log4c_category_log(runlog, LOG4C_PRIORITY_ERROR, "Hello World!");

	/*  Explicitly call the log4c cleanup routine */
	if ( log4c_fini()){                                                                                                                           
		printf("log4c_fini() failed");
		return;
	}   
}

enum BOOL { FALSE=0, TRUE=1 };

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  test_driver
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	CU_TestInfo cases_a[] = {	/* 定义测试集合A的三个测试用例 */
		{"模块A-测试用例1", test_a_1},
		{"模块A-测试用例2", test_a_2},
		{"模块A-测试用例3", test_a_3},
		CU_TEST_INFO_NULL
	};

	CU_TestInfo cases_b[] = {	/* 定义测试集合B的三个测试用例 */
		{"模块B-测试用例1", test_b_1},
		{"模块B-测试用例2", test_b_2},
		{"模块B-测试用例3", test_b_3},
		CU_TEST_INFO_NULL
	};

	CU_TestInfo cases_c[] = {	/* 定义测试集合C的三个测试用例 */
		{"模块C-测试用例1", test_c_1},
		{"模块C-测试用例2", test_c_2},
		{"模块C-测试用例3", test_c_3},
		CU_TEST_INFO_NULL
	};

	/* 说明，此处并没有出现CU_pSuite变量，这是因为cunit是以“描述”作为测试用例或测试集合的唯一标识，而非变量 */
	CU_SuiteInfo suites[] = {	/* 定义三个测试集合 */
		{"测试集合A", test_a_init, test_a_destroy, cases_a},
		{"测试集合B", test_b_init, test_b_destroy, cases_b},
		{"测试集合C", test_c_init, test_c_destroy, cases_c},
		CU_SUITE_INFO_NULL
	};

	/* 初始化cunit */
	if (CU_initialize_registry() == CUE_NOMEMORY) {
		fprintf(stderr, "CU_initialize_registry fault!\n");
		exit(EXIT_FAILURE);
	}

	/* 向cunit注册测试集合 */
	if (CU_register_suites(suites) != CUE_SUCCESS) {
		fprintf(stderr, "CU_register_suites fault!\n");
	}

	/* 设置测试集合的激活标志 */
	CU_set_suite_active(CU_get_suite("测试集合A"), TRUE);	/* 激活测试集合A */
	CU_set_suite_active(CU_get_suite("测试集合B"), FALSE);	/* 不激活测试集合B */
	CU_set_suite_active(CU_get_suite("测试集合C"), TRUE);	/* 激活测试集合C */

	/* 设置测试用例的激活标志 */
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合A"), "模块A-测试用例1"), TRUE);
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合A"), "模块A-测试用例2"), TRUE);
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合A"), "模块A-测试用例3"), TRUE);
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合B"), "模块B-测试用例1"), TRUE);	/* 由于测试集合B未激活，所以模块B的测试用例不会执行 */
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合B"), "模块B-测试用例2"), TRUE);	/* 由于测试集合B未激活，所以模块B的测试用例不会执行 */
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合B"), "模块B-测试用例3"), TRUE);	/* 由于测试集合B未激活，所以模块B的测试用例不会执行 */
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合C"), "模块C-测试用例1"), TRUE);
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合C"), "模块C-测试用例2"), FALSE);	/* 不激活"模块C-测试用例2" */
	CU_set_test_active(CU_get_test(CU_get_suite("测试集合C"), "模块C-测试用例3"), TRUE);

	/* 执行测试 */
	CU_basic_run_tests();	/* 最基本的执行方式，非交互，将测试结果输出到屏幕 */
	/* xml方式：将测试结果输出到当前目录下，生成两个xml文件，需要结合库文件中6个xml的配套文件来查看测试结果 */
	/*
	CU_set_output_filename("test");
	CU_list_tests_to_file();
	CU_automated_run_tests();
	*/

	/* 清理cunit注册器，防止内存泄漏 */
	CU_cleanup_registry();

	log4c_demo();	/* 介绍log4c的用法 */

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

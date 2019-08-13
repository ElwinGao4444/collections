/*
 * =====================================================================================
 *
 *       Filename:  errno.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/27/2015 02:47:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdarg.h>

#include <cstdlib>
#include <iostream>
#include <ext/hash_map>

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

using __gnu_cxx::hash_map;

class Error_Wrapper
{
public:
	Error_Wrapper(int num, std::string msg)
	{
		error_map[num] = msg;
	}

	static std::string& errmsg(int num)
	{
		return error_map[num];
	}

private:
	static hash_map<int, std::string> error_map;
};

static std::string& errmsg(int num)
{
	return Error_Wrapper::errmsg(num);
}

static std::string errmsg(int num, const char *format, ...)
{
	char buf[64<<10];
	va_list ap;
	va_start(ap, format);
	int n = vsnprintf(buf, 64<<10, format, ap);
	va_end(ap);

	std::string result;
	result.append(Error_Wrapper::errmsg(num));
	result.append(": ");
	result.append(buf);
	return result;
}

#define ERROR_DEFINE(name, num, msg) enum {name = num};
#include "errdef.h"
#undef ERROR_DEFINE


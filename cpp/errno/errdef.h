/*
 * =====================================================================================
 *
 *       Filename:  errdef.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/27/2015 02:49:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

/* 
 * 特别注意：此头文件不能使用#ifndef宏，否则会造成错误信息无法注册的问题
 */

ERROR_DEFINE(SUCCESS, 0, "success msg")
ERROR_DEFINE(FAILURE, -1, "failure msg")

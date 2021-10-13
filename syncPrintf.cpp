/*
 * synchronizedPrintf.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include <STDIO.H>
#include <STDARG.H>
#include "kernel.h"

int synchronizedPrintf(const char *format, ...){
	va_list args;

	systemLock();

	va_start(args, format);
	int res = vprintf(format, args);
	va_end(args);
	fflush(stdout);
	systemUnlock();

	return res;
}



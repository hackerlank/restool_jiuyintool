#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include "define.h"
using namespace std;

float fmt(float n);

class Util
{
public:
	static bool mkdir(const char* path, bool dirflag = false);
	static void fname(const char* path, char* dest);
	static void fpath(const char* path, char* dest);
	static void fcopy(const char* path, const char* dest, bool delflag = false);
	static uint32 crc32(const char* pData, int nByteCount);
};


#endif //__UTIL_H__


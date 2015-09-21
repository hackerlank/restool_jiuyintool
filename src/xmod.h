#ifndef __XMOD_H__
#define __XMOD_H__

#include "define.h"


#pragma pack(4)
typedef struct _XmodHead
{
    char   filetag[10]; 
    char   version[20];

	uint32 filecount;
	uint32 tbloffset;

    uint8  encrypt;  
    uint8  reserve[32];  

} XmodHead;

#pragma pack()


class Xmod
{
public:
	Xmod(const char* path);
	~Xmod();

	void info();
	void save();
public:
	char 	 _path[255];
	ifstream _file;

	XmodHead _head;
};



#endif


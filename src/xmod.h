#ifndef __PKG_H__
#define __PKG_H__

#include "define.h"


#pragma pack(4)
typedef struct _PkgHead
{
    char   filetag[10]; 
    char   version[20];

	uint32 filecount;
	uint32 tbloffset;

    uint8  encrypt;  
    uint8  reserve[32];  

} PkgHead;

typedef struct _PkgFile
{
    char   path[260];
    char   md5[33];
    uint32 offset;
    uint32 rawsize;
    uint32 zipsize;
    uint8  ziptype;
    uint8  weakuse;

} PkgFile;

#pragma pack()


class Pkg
{
public:
	Pkg(const char* filename);
	~Pkg();

	void info();
	void save();
public:
	char 	 _path[255];
	ifstream _file;

	PkgHead         _head;
	vector<PkgFile> _list;
};



#endif


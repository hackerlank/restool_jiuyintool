#ifndef __XMOD_H__
#define __XMOD_H__

#include "define.h"


#pragma pack(4)
typedef struct _Bind
{
    int     b1;
    float   w1;
    int     b2;
    float   w2;
    int     b3;
    float   w3;
    int     b4;
    float   w4;
} Bind;

typedef struct _Obj
{
    char name[255];

    vector<float>   vert;
    vector<short>   face;
    vector<Bind>    bind;
    vector<string>  texs;
} Obj;

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

    uint32 _tag;
    uint32 _ver;
    char   _name[255];
    vector<Obj> _objs;
};



#endif


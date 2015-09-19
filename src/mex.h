#ifndef __MEX_H__
#define __MEX_H__

#include "define.h"


#pragma pack(4)
typedef struct _Mtl
{
    vector<int>     blend;
    vector<int>     flags;
    vector<int>     texid;
    vector<float>   alpha;
} Mtl;

typedef struct _Face
{
    uint16 ids[3];
} Face;

typedef struct _Geo
{
    char name[80];
    int vert;
    int face;
    int mtlid;
    int lightmap;

    vector<vec3>           vertList;
    vector<uint32>         colorList;
    vector<vec3>           normalList;
    vector< vector<vec2> > uvList;
    vector<uint8>          boneList;
    vector<Face>           faceList;
} Geo;

typedef struct _Tcb
{
    float tension;
    float continuity;
    float bias;
} Tcb;

typedef struct _KeyFrame
{
    int id;
    int posSpace;
    int rotSpace;

    vec3 trans;
    quat rotat;

    int tcbIndex;
} KeyFrame;

typedef struct _Bone
{
    char name[80];
    int id;
    int parent;
    vector<int> children;

    uint8 type;
    vector<mat4> frameList;
    vector<vec3> transList;
    vector<quat> rotatList;

    bool isTcb;
    vec3 initTrans;
    quat initRotat;

    vec3 poseTrans;
    quat poseRotat;
    vector<KeyFrame> keyList;
    vector<int>      keyIndex;
} Bone;

typedef struct _Attach
{
    char name[80];
    int  bone;
    mat4 mat;
} Attach;

#pragma pack()


class Mex
{
public:
	Mex(const char* filename);
	~Mex();

	void info();
	void save();
public:
	char 	 _path[255];
	ifstream _file;

    int _specialModel;//特殊坐骑，逐帧动画

    int              _version;
    vector<char*>    _texs;
    vector<Mtl>      _mtls;
    vector<Geo>      _geos;

    vector<int>      _boneRoot;
    vector<Bone>      _boneList;

    vector<Tcb>      _tcbList;
    vector<int>      _keyList;

    vector<Attach>   _attach;

};



#endif


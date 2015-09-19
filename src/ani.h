#ifndef __ANI_H__
#define __ANI_H__

#include "define.h"


#pragma pack(4)
typedef struct _HitPoint
{
    int frameId;
    char wavFile[256];

    int hitTraget;
    int srcEffect;
    int dstEffect;
} HitPoint;

typedef struct _Sequence
{
    char animName[256];
    uint32 startFrameId;
    uint32 endFrameId;

    vector<HitPoint> hitPoints;
    int realHitPoint;
    int hasLightTrack;
} Sequence;

typedef struct _SoundEffect
{
    char filename[256];
    int range;
    int loopflag;
    int frequence;
    int intensity;
} SoundEffect;

typedef struct _BoundingObj
{
    int bboxWidth;
    int bboxLength;
    int bboxHeight;
    int bboxLift;
} BoundingObj;

typedef struct _Setting
{
    char name[256];

    vector<uint32> subsetFlags;
    vector<char*>  replaceTexs;

} Setting;

typedef struct _BodyRadius
{
    float radius;
    float f1;
    float f2;
    float f3;
    float f4;
} BodyRadius;

#pragma pack()


class Ani
{
public:
	Ani(const char* filename);
	~Ani();

	void info();
	void save();
public:
	char 	 _path[255];
	ifstream _file;


    vector<Sequence> _sequences;
    int              _modelscale;
    SoundEffect      _soundeffect;
    BoundingObj      _boundingobj;
    vector<Setting>  _avatarsets;
    BodyRadius       _bodyradius;
    int              _iscomponent;
};



#endif


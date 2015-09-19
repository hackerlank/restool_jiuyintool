#include "util.h"
#include "ani.h"


Ani::Ani(const char* filename)
{
	strcpy(_path, filename);
	_file.open(filename, ios::in|ios::binary);
	_file.seekg(0);

    uint32 ftag;
    uint32 size;

    int n;
    int idx;
    char* tex;
    Setting ss;
    HitPoint hitp;
    Sequence *sqce;

    while(_file.good())
    {
	    _file.read((char *)&ftag, 4);
	    _file.read((char *)&size, 4);
        switch(FTAG(ftag))
        {
        case 'mexa':
            break;

        case 'sqcs':
            break;

        case 'sqce':
            idx = _sequences.size();
            _sequences.resize(idx+1);
            sqce = &(_sequences[idx]);

	        _file.read((char *)(sqce->animName), 256);
	        _file.read((char *)&(sqce->startFrameId), 4);
	        _file.read((char *)&(sqce->endFrameId), 4);
            break;

        case 'htps':
            break;

        case 'hitp':
            
	        _file.read((char *)&hitp.frameId, 4);
	        _file.read((char *)hitp.wavFile, 256);
            if(size > 256 + 4)
            {
	            _file.read((char *)&hitp.hitTraget, 4);
	            _file.read((char *)&hitp.srcEffect, 4);
	            _file.read((char *)&hitp.dstEffect, 4);
            }
            sqce->hitPoints.push_back(hitp);
            break;

        case 'rlht':
	        _file.read((char *)&(sqce->realHitPoint), 4);
            break;

        case 'lttk':
	        _file.read((char *)&(sqce->hasLightTrack), 4);
            break;
    
        case 'mxse':
	        _file.read((char *)&_modelscale, 4);
            break;

        case 'sond':
	        _file.read((char *)_soundeffect.filename, 256);
	        _file.read((char *)&_soundeffect.range, 4);
	        _file.read((char *)&_soundeffect.loopflag, 4);
	        _file.read((char *)&_soundeffect.frequence, 4);
	        _file.read((char *)&_soundeffect.intensity, 4);
            break;
        case 'bbox':
	        _file.read((char *)&_boundingobj.bboxWidth, 4);
	        _file.read((char *)&_boundingobj.bboxLength, 4);
	        _file.read((char *)&_boundingobj.bboxHeight, 4);
	        _file.read((char *)&_boundingobj.bboxLift, 4);
            break;
        case 'arss':
            break;
        case 'arsg':
	        _file.read((char *)ss.name, 256);

	        _file.read((char *)&n, 4);
            if(n > 0)
            {
                ss.subsetFlags.resize(n);
	            _file.read((char *)&ss.subsetFlags[0], 4 * n);
            }
            
            _file.read((char *)&n, 4);
            if(n > 0)
            {
                for(int i = 0; i < n; i++)
                {
                    tex = new char[260];
	                _file.read((char *)tex, 260);
                    ss.replaceTexs.push_back(tex);
                }
            }
            break;
        case 'byrs':
	        _file.read((char *)&_bodyradius, sizeof(_bodyradius));
            break;
        case 'cmpt':
	        _file.read((char *)&_iscomponent, 4);
            break;
        default:
	        _file.seekg(_file.tellg() - (streamoff)4);
            break;
        }
    }

}


Ani::~Ani()
{
	_file.close();
}


void Ani::info()
{
	cout << "=================================================" << endl;
	cout << "[Head]" << _path << endl;
    cout << "sequences   " << dec << showbase << _sequences.size() << endl; 
    cout << "modelscale  " << dec << showbase << _modelscale << endl; 
    cout << "soundeffect " << dec << showbase << _soundeffect.filename << endl; 
    cout << "boundingobj " << dec << showbase << _boundingobj.bboxWidth << endl;  
    cout << "avatarsets  " << dec << showbase << _avatarsets.size() << endl; 
    cout << "bodyradius  " << dec << showbase << _bodyradius.radius << endl; 
    cout << "iscomponent " << dec << showbase << _iscomponent << endl; 
	cout << "=================================================" << endl;
    cout << dec << showbase << endl;


    for(int i = 0; i < _sequences.size(); i++)
    {
        Sequence s = _sequences[i];
        cout << s.animName << " " << s.startFrameId << " " << s.endFrameId << endl;
    }
}

void Ani::save()
{

}








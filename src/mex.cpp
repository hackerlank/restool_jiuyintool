#include "util.h"
#include "mex.h"

#define stricmp strcasecmp

Mex::Mex(const char* filename)
{
	strcpy(_path, filename);
	_file.open(filename, ios::in|ios::binary);
	_file.seekg(0);

    uint32 ftag;
    uint32 size;
    
    streamoff pos;
    int n;
    int ni;
    float nf;
    char* p;
    mat4 mat;
    Mtl* mtl;
    Geo* geo;
    Bone* bone;
    Attach ath;
    KeyFrame frame;

    while(_file.good())
    {
	    _file.read((char *)&ftag, 4);
	    _file.read((char *)&size, 4);
        switch(FTAG(ftag))
        {
        case 'vers':
	        _file.read((char *)&_version, 4);
            break;
        case 'texs':
	        _file.read((char *)&n, 4);
            for(int i = 0; i < n; i++)
            {
                p = new char[260];
                _file.read(p, 260);
                _texs.push_back(p);
            }
            break;
        case 'mtls':
            break;
        case 'matl':
            n = _mtls.size();
            _mtls.resize(n + 1);
            mtl = &_mtls[n];
            break;
        case 'lays':
	        _file.read((char *)&n, 4);
            for(int i = 0; i < n; i++)
            {
	            _file.read((char *)&ni, 4);
                mtl->blend.push_back(ni);
	            _file.read((char *)&ni, 4);
                mtl->flags.push_back(ni);
	            _file.read((char *)&ni, 4);
                mtl->texid.push_back(ni);
	            _file.read((char *)&nf, 4);
                mtl->alpha.push_back(nf);
            }
            break;
        case 'geom':
	        _file.read((char *)&n, 4);//0
            break;
        case 'chks':
            n = _geos.size();
            _geos.resize(n + 1);
            geo = &_geos[n];

	        _file.read((char *)&(geo->vert), 4);
	        _file.read((char *)&(geo->face), 4);
	        _file.read((char *)&(geo->mtlid), 4);

            geo->vertList.resize(geo->vert);
	        _file.read((char *)&(geo->vertList[0]), sizeof(vec3) * geo->vert);

            geo->normalList.resize(geo->vert);
	        _file.read((char *)&(geo->normalList[0]), sizeof(vec3) * geo->vert);

            geo->uvList.resize(1);
            geo->uvList[0].resize(geo->vert);
	        _file.read((char *)&(geo->uvList[0][0]), sizeof(vec2) * geo->vert);

            geo->boneList.resize(geo->vert);
	        _file.read((char *)&(geo->boneList[0]), sizeof(uint8) * geo->vert);

            geo->faceList.resize(geo->face);
	        _file.read((char *)&(geo->faceList[0]), sizeof(Face) * geo->face);

            break;
        case 'mtuv':
            pos = _file.tellg() + (streamoff)size;
	        _file.read((char *)&n, 4);
            geo->uvList.resize(n);
            for(int i = 1; i < n; i++)
            {
                geo->uvList[i].resize(geo->vert);
	            _file.read((char *)&(geo->uvList[i][0]), sizeof(vec2) * geo->vert);
            }
            if(_file.tellg() < pos)
	            _file.read((char *)&(geo->lightmap), 4);
            else
                geo->lightmap = 0;

            break;
        case 'vcol':
            geo->colorList.resize(geo->vert);
	        _file.read((char *)&(geo->colorList[0]), sizeof(uint32) * geo->vert);
            break;
        case 'name':
	        _file.read((char *)&(geo->name), 80);
            break;
        case 'sklt':
	        _file.read((char *)&n, 4);
	        _file.read((char *)&n, 4);
            for(int i = 0; i < n; i++)
            {
	            _file.read((char *)&ni, 4);
                _boneRoot.push_back(ni);
            }

            if(_version >= 108)
            {
	            _file.read((char *)&n, 4);
                if(n == -1)
                {
                    _specialModel = 1;
                }
                else
                {
                    _specialModel = 0;
                    for(int i = 0; i < n; i++)
                    {
	                    _file.read((char *)&ni, 4);
                        _keyList.push_back(ni);
                    }
                }
	            _file.read((char *)&n, 4);
                if(n > 0)
                {
                    _tcbList.resize(n);
                    for(int i = 0; i < n; i++)
                    {
	                    _file.read((char *)&nf, 4);
                        nf *= 0.01f;
                        _tcbList[i].tension = nf;
                        _file.read((char *)&nf, 4);
                        nf *= 0.01f;
                        _tcbList[i].continuity = nf;
                        _file.read((char *)&nf, 4);
                        nf *= 0.01f;
                        _tcbList[i].bias = nf;
                    }
                }
            }
            break;
        case 'bone':
            n = _boneList.size();
            _boneList.resize(n + 1);
            bone = &_boneList[n];

            _file.read((char*)&(bone->name), 80);
            _file.read((char*)&(bone->parent), 4);
            _file.read((char*)&n, 4);
            for(int i = 0; i < n; i++)
            {
                _file.read((char*)&ni, 4);
                bone->children.push_back(ni);
            }
            break;
        case 'trck':
            bone->type = 1;
            _file.read((char*)&n, 4);
            for(int i = 0; i < n; i++)
            {
                _file.read((char*)&mat, sizeof(mat));
                bone->frameList.push_back(mat);
            }
            break;
        case 'trk2':
            _file.read((char*)&n, 4);
            if(_version < 108 || _specialModel || 
                (stricmp(bone->name, "Bip01") == 0) ||
                (stricmp(bone->name, "Bip01 L Thigh") == 0) ||
                (stricmp(bone->name, "Bip01 L Calf") == 0) ||
                (stricmp(bone->name, "Bip01 L Foot") == 0) ||
                (stricmp(bone->name, "Bip01 L Toe0") == 0) ||
                (stricmp(bone->name, "Bip01 R Thigh") == 0) || 
                (stricmp(bone->name, "Bip01 R Calf" ) == 0) ||
                (stricmp(bone->name, "Bip01 R Foot" ) == 0) || 
                (stricmp(bone->name, "Bip01 R Toe0" ) == 0))
            {
                bone->type = 2;
                bone->transList.resize(n);
                _file.read((char*)&(bone->transList[0]), sizeof(vec3) * n);
                bone->rotatList.resize(n);
                _file.read((char*)&(bone->rotatList[0]), sizeof(quat) * n);
            }else
            {
                bone->type = 3;
                _file.read((char*)&(bone->initTrans), sizeof(vec3));
                _file.read((char*)&(bone->initRotat), sizeof(quat));
                _file.read((char*)&(bone->poseTrans), sizeof(vec3));
                _file.read((char*)&(bone->poseRotat), sizeof(quat));

                _file.read((char*)&n, 4);//key frame count
                _file.read((char*)&ni, 4);//tcb
                bone->isTcb = (ni > 0);

                for(int i = 0; i < n; i++)
                {
                    _file.read((char*)&frame.id, 4);
                    _file.read((char*)&frame.posSpace, 4);
                    _file.read((char*)&frame.rotSpace, 4);
                    _file.read((char*)&frame.trans, sizeof(vec3));
                    _file.read((char*)&frame.rotat, sizeof(quat));
                    if(bone->isTcb)
                    {
                        _file.read((char*)&frame.tcbIndex, 4);
                    }
                    bone->keyList.push_back(frame);
                }
                if(n > 0)
                {
                    _file.read((char*)&n, 4);
                    for(int i = 0; i < n; i++)
                    {
                        _file.read((char*)&ni, 4);
                        bone->keyIndex.push_back(ni);
                    }
                }
            }
            break;
        case 'atts':
            break;
        case 'att0':
	        _file.read((char *)&ath.bone, 4);
	        _file.read((char *)&ath.name, 80);
	        _file.read((char *)&ath.mat, sizeof(ath.mat));
            _attach.push_back(ath);
        default:
            _file.seekg(_file.tellg() + (streamoff)size);
            break;
        }
    }

}


Mex::~Mex()
{
	_file.close();
}


void Mex::info()
{
	cout << "=================================================" << endl;
	cout << "[Head]" << _path << endl;
    cout << "version " << dec << showbase << _version << endl; 
    cout << "texs    " << dec << showbase << _texs.size() << endl; 
    cout << "mtls    " << dec << showbase << _mtls.size() << endl; 
    cout << "geos    " << dec << showbase << _geos.size() << endl; 
    cout << "boneRoot   " << dec << showbase << _boneRoot.size() << endl; 
    cout << "boneList   " << dec << showbase << _boneList.size() << endl; 
    cout << "tcbList    " << dec << showbase << _tcbList.size() << endl; 
    cout << "attach  " << dec << showbase << _attach.size() << endl; 
	cout << "=================================================" << endl;
    cout << dec << showbase << endl;


    for(int i = 0; i < _texs.size(); i++)
        cout << "tex " << _texs[i] << endl;
    for(int i = 0; i < _geos.size(); i++)
    {
        Geo g = _geos[i];
        cout << "geo " << g.name << " vert " << g.vertList.size() << " face " << g.faceList.size() << " bone " << g.boneList.size() << endl;
    }
    for(int i = 0; i < _boneList.size(); i++)
    {
        Bone b = _boneList[i];
        cout << "bone " << b.name << " type " << (int)b.type << " frame " << b.frameList.size() << " trans " << b.transList.size() 
            << " roatat " << b.rotatList.size() << " key " << b.keyList.size() << " idx " << b.keyIndex.size() << endl;
    }
    for(int i = 0; i < _attach.size(); i++)
        cout << "attach " << _attach[i].name << endl;
}

void Mex::save()
{

}








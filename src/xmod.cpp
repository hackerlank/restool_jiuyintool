#include "util.h"
#include "xmod.h"


Xmod::Xmod(const char* path)
{
	strcpy(_path, path);
	_file.open(_path, ios::in|ios::binary);

	_file.seekg(0);
	_file.read((char *)&_tag, 4);
	_file.read((char *)&_ver, 4);

    uint32 n;
    uint16 s;
    float f;
    char c;

	_file.read((char *)&n, 4);//ukn1
	_file.read((char *)&n, 4);//ukn2

    memset(&_name, 0, sizeof(_name));
	_file.read((char *)&n, 4);
	_file.read((char *)&_name, n);

	_file.read((char *)&n, 4);//ukn3
	_file.read((char *)&n, 4);//num1

    for(int i = 0; i < 7; i++)//f_unk1[7]
    {
	    _file.read((char *)&f, 4);
        //cout << f;
    }

	_file.read((char *)&n, 4);//numObj
    cout << "objs " << n << endl;
    for(int i = n; i > 0; i--)
    {
        Obj obj;
        memset(&obj, 0, sizeof(obj));

	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.name, n);


        uint16 num2, t1, t2;
	    _file.read((char *)&num2, 4);//num2
	    _file.read((char *)&t1, 2);//type1
	    _file.read((char *)&t2, 2);//type2
        cout << hex << showbase;
        cout << "t1 " << t1 << " t2 " << t2 << endl;

        if(t2 == 0x00)
        {
            for(int i = 0; i < 44; i++)
	            _file.read((char *)&c, 1);
        }else
        {
            for(int i = 0; i < 72; i++)
	            _file.read((char *)&c, 1);
        }

	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.mtlName, n);
        cout << obj.mtlName << endl;
        cout << "mtls end at " << hex << showbase << _file.tellg() << endl;

        uint32 ukn6, ukn7;
	    _file.read((char *)&ukn6, 4);//ukn6
	    _file.read((char *)&ukn7, 4);//ukn7
        cout << "ukn6 " << ukn6 << " ukn7 " << ukn7 << endl;
        cout << dec << showbase;

        int vp = 0;
        if(t1 == 0x1505)
            vp = 14;
        else if(t1 == 0x1015)
            vp = 9;
        else if (t1 == 0x1005)
        {
            if(num2 == 0x3)
                vp = 3;
            else
            {
                if(ukn7 == 0x5 || ukn7 == 0x405)
                    vp = 6;
                else
                    vp = 8;
            }

        }else
        {
            cout << "[ERROR]Unknown type1 " << hex << showbase << t1 << endl;
            break;
        }

        cout << "vert start at " << hex << showbase << _file.tellg() << endl;
        
        uint32 vertNum;
        if(t2 > 0x00)
        {
	        _file.read((char *)&vertNum, 4);

            cout << "vert " << vertNum << " vp " << vp << endl;
            obj.vert.resize(vertNum * vp);
	        _file.read((char *)&(obj.vert[0]), vertNum * vp * 4);
            cout << "vert end at " << hex << showbase << _file.tellg() << endl;

            uint32 faceNum;
	        _file.read((char *)&faceNum, 4);
            cout << "face " << faceNum << endl;
            obj.face.resize(faceNum);
	        _file.read((char *)&(obj.face[0]), faceNum * 2);
            cout << "face end at " << hex << showbase << _file.tellg() << endl;
        }

        if(t2 == 0x01)
        {
            obj.bind.resize(vertNum);
	        _file.read((char *)&(obj.bind[0]), vertNum * sizeof(Bind));
            cout << "bind end at " << hex << showbase << _file.tellg() << endl;

            //for(int i = 0; i < vertNum; i++)
            //{
            //    Bind b = obj.bind[i];
            //    cout << dec << showbase << "bind " << b.b1 << " " << b.w1
            //    <<" " << b.b2 << " " << b.w2
            //    <<" " << b.b3 << " " << b.w3
            //    <<" " << b.b4 << " " << b.w4 << endl;
            //}



	        _file.read((char *)&n, 4);
            int  bc = 0;
            int  bi = 0;
            char bname[255];
            for(int i = 0; i < n; i++)
            {
	            _file.read((char *)&c, 1);
                bname[bi] = c;
                if(c != '\0')
                {
                    bi++;
                }
                else
                {
                    bc++;
                    bi = 0;
                    //cout << bname << endl;
                    //cout << i << endl;
                }
            }
        }

        for(int i = 0; i < 6; i++)
	        _file.read((char *)&f, 4);
        if(ukn7 == 0x1405)
	        _file.read((char *)&n, 4);


	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.texName, n);
        cout << obj.texName << endl;

	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.texFile, n);
        cout << obj.texFile << endl;


        if(t2 == 0x01)
        {
	        _file.read((char *)&n, 4);
            for(int i = n; i > 0; i--)
            {
                char uname[255];
                memset(uname, 0, 255);
	            _file.read((char *)&n, 4);
	            _file.read((char *)&uname, n);
                //cout << uname << endl;
	            
                memset(uname, 0, 255);
                _file.read((char *)&n, 4);
	            _file.read((char *)&uname, n);
                //cout << uname << endl;

                for(int j = 0; j < 48; j++)
                {
	                _file.read((char *)&c, 1);
                }
            }
        }


        for(int i = 0; i < 4; i++)
	        _file.read((char *)&c, 1);

        cout << "obj end at " << hex << showbase << _file.tellg() << endl;

        _objs.push_back(obj);
    }

}


Xmod::~Xmod()
{
	_file.close();
}


void Xmod::info()
{
	cout << "[Info]" << _path << endl;
    cout << "tag " << hex << showbase << _tag << endl; 
    cout << "ver " << hex << showbase << _ver<< endl; 
    cout << "obj " << dec << showbase << _objs.size() << endl; 
	cout << "-------------------------" << dec << showbase << endl;
}

void Xmod::save()
{

}








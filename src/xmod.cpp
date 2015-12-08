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
    uint32 ukn1, ukn2, ukn3, num1, num2, num3;

	_file.read((char *)&ukn1, 4);//0x2
	_file.read((char *)&ukn2, 4);//0x1


    memset(&_name, 0, sizeof(_name));
	_file.read((char *)&n, 4);
	_file.read((char *)_name, n);

	_file.read((char *)&ukn3, 4);//[0,1]
	_file.read((char *)&num1, 4);//num1

    _file.seekg(28, ios::cur);//7f

    int check = 0;
	_file.read((char *)&check, 4);
    while(check != -1)
    {
        _file.seekg(-3, ios::cur);
	    _file.read((char *)&check, 4);

        if(!_file.good())
            return;
    }

    _file.seekg(24, ios::cur);//6f

	_file.read((char *)&n, 4);//numObj
    cout << "objs " << n << " " << hex << showbase << _file.tellg() << dec << showbase << endl;
    for(int i = n * 2; i > 0; i--)
    {
        Obj obj;
        memset(&obj, 0, sizeof(obj));

	    _file.read((char *)&n, 4);
	    _file.read((char *)obj.name, n);


	    _file.read((char *)&num3, 4);//num3 0x0
        char mb[4];
	    _file.read((char *)&mb, 4);

        uint32 vertn;
       	_file.read((char *)&vertn, 4);


        uint32 facen;
        int init = _file.tellg();

        //test vert
        int vlen = 12;
        for(; vlen < 128; vlen += 4)
        {
            _file.seekg(init + vlen * vertn);

	        _file.read((char *)&facen, 4);
            if(!_file.good())
                return;

            if(facen % 3 == 0 && facen + 64 > vertn && facen < vertn * 6 + 64)
            {
                cout << "guess " << facen << " " << vertn << endl;
                int t;
                for(t = 0; t < facen/3; t++)
                {
                    uint16 idx[3];
	                _file.read((char *)idx, 6);
                    if(!_file.good())
                        break;
                    if(idx[0] > vertn || idx[1] > vertn || idx[2] > vertn)
                        break;
                    if(idx[0] == idx[1] && idx[1] == idx[2])
                        break;
                }
                if(t >= facen/3)
                {
                    cout << "facen " << facen << endl;
                    cout << "vlen " << vlen << endl;
                    obj.vertLen = vlen;
                    break;
                }
            }
        }
        if(vlen >= 128)
            return;
        
        //test bone

        bool hasBone = true;
        for(int i = 0; i < vertn; i++)
        {
            Bind bb;
	        _file.read((char *)&bb, sizeof(Bind));
            if(!_file.good())
                return;

            if( bb.b1 < -1 || bb.b2 < -1 || bb.b3 < -1 || bb.b4 < -1 ||
                bb.b1 > 999 || bb.b2 > 999 || bb.b3 > 999 || bb.b4 > 999 ||
                bb.w1 + bb.w2 + bb.w3 + bb.w4 > 1.01f ||
                bb.w1 + bb.w2 + bb.w3 + bb.w4 < -0.01f)
            {
                hasBone = false;
                break;
            }
        }
        cout << "bone  " << int(hasBone) << endl;


        //read data
        _file.seekg(init);
        obj.vertn = vertn;
        for(int i = 0; i < vertn*vlen/4; i++)
        {
            float f;
	        _file.read((char *)&f, 4);
            obj.vert.push_back(f);
        }

	    _file.read((char *)&facen, 4);
        obj.facen = facen;
        for(int i = 0; i < facen; i++)
        {
            uint16 s;
	        _file.read((char *)&s, 2);
            obj.face.push_back(s);
        }

        if(hasBone)
        {
            for(int i = 0; i < vertn; i++)
            {
                Bind b;
	            _file.read((char *)&b, sizeof(Bind));
                obj.bind.push_back(b);
            }
	        
            _file.read((char *)&n, 4);
            obj.boneName = new char[n];
            _file.read((char *)obj.boneName, n);
            cout << "boneName " << obj.boneName << endl;
        }




        //test string
        int last = _file.tellg();
        while(true)
        {
            int mark = _file.tellg();
            int i;
            for(i = 0; i < 9999; i += 4)
            {
                bool isStr = true;
                char ss[255];

                _file.seekg(mark + i, ios::beg);
                if(!_file.good())
                {
                    i = 9999;
                    break;
                }

                _file.read((char *)&n, 4);
                if(!_file.good())
                {
                    i = 9999;
                    break;
                }

                if(n > 4 && n < 256)
                {
                    memset(ss, 0, 255);
                    _file.read(ss, n);
                    if(!_file.good())
                        break;

                    for(int j = 0; j < n; j++)
                    {
                        char c = ss[j];
                        if((c >= 'a' && c <= 'z') ||
                           (c >= 'A' && c <= 'Z') ||
                           (c >= '0' && c <= '9') ||
                           c == '.' || c == '_' || 
                           c == ' ' || c == ':' || 
                           c == '+' || c == '@' || 
                           c == '-' || c == '#')
                        {
                            //ok;
                        }else
                        {
                            isStr = false;
                        }
                    }

                }else
                {
                    isStr = false;
                }

                if(isStr)
                {
                    cout << ss << endl;
                    obj.texs.push_back(ss);
                    last = mark + i;
                    break;
                }
            }
            if(i >= 9999)
                break;
        }
        _file.seekg(last, ios::beg);


        _objs.push_back(obj);
        cout << "[OBJ]" << obj.name << " vertn " << obj.vertn << endl;
        cout << "obj ends " << hex << showbase << last << " " << _file.tellg() << dec << showbase << endl;
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
    for(int i = 0; i < _objs.size(); i++)
    {
        Obj obj = _objs[i];

        char path[255];
        strcpy(path, "out/");
        strcpy(path+4, obj.name);
        ofstream fout(path, ios::out|ios::binary);


        uint16 vertN = (uint16)obj.vertn;
        fout.write((char*)&vertN, 2);
        cout << "vert " << vertN << endl; 
        for(int p = 0; p < obj.vert.size(); p += (obj.vertLen/4))
        {
            for(int t = 0; t < 3; t++)
            {
                float f = obj.vert[p + t] * 100;
                fout.write((char*)&f, 4);
                //cout << f << " ";
            }
            //cout << endl;

            float u = obj.vert[p + (obj.vertLen/4) - 2];
            fout.write((char*)&u, 4);
            float v = 1.0f - obj.vert[p + (obj.vertLen/4) - 1];
            fout.write((char*)&v, 4);

            uint8 bindN = 1;
            fout.write((char*)&bindN, 1);
            fout.write((char*)&bindN, 1);
            float bindW = 1.0f;
            fout.write((char*)&bindW, 4);
        }



        uint16 faceN = (uint16)obj.facen/3;
        fout.write((char*)&faceN, 2);
        cout << "face " << faceN << endl; 
        for(int p = 0; p < obj.face.size(); p++)
        {
            uint16 idx = obj.face[p];
            fout.write((char*)&idx, 2);
            //cout << idx << " ";
        }
        //cout << endl;

        fout.close();
    }

}








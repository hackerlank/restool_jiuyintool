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
    for(int i = n; i > 0; i--)
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
        cout << dec << showbase << "vert " << vertn << endl;


        uint32 facen;
        int init = _file.tellg();
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
                    break;
                }
            }
        }
        if(vlen >= 128)
            return;

        cout << "ends " << hex << showbase << _file.tellg() << dec << showbase << endl;

        return;


	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.mtlName, n);
        cout << obj.mtlName << endl;

	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.texName, n);
        cout << obj.texName << endl;

	    _file.read((char *)&n, 4);
	    _file.read((char *)&obj.texFile, n);
        cout << obj.texFile << endl;



        _objs.push_back(obj);
        cout << "obj end at " << hex << showbase << _file.tellg() << endl;
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








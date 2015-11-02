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

	_file.read((char *)&n, 4);//ukn1
	_file.read((char *)&n, 4);//ukn2


    memset(&_name, 0, sizeof(_name));
	_file.read((char *)&n, 4);
	_file.read((char *)_name, n);

	_file.read((char *)&n, 4);//ukn3
    uint32 num1;
	_file.read((char *)&num1, 4);//num1

    _file.seekg(28, ios::cur);//7f

    char sname[255];


	_file.read((char *)&n, 4);
	_file.read((char *)&n, 4);
	_file.read((char *)sname, n);

	_file.read((char *)&n, 4);//num2

    char vb[4];
	_file.read((char *)&vb, 4);

    int check = 0;
	_file.read((char *)&check, 4);
    while(check != -1)
    {
        //char sname[255];

	    //_file.read((char *)&n, 4);
	    //_file.read((char *)sname, n);

	    //_file.read((char *)&n, 4);//num2

        //char vb[4];
	    //_file.read((char *)&vb, 4);

        //_file.seekg(40, ios::cur);//10f

        _file.seekg(-3, ios::cur);
	    _file.read((char *)&check, 4);

        if(!_file.good())
            return;
    }

    _file.seekg(24, ios::cur);//6f

	_file.read((char *)&n, 4);//numObj
    cout << "objs " << n << endl;
    for(int i = n; i > 0; i--)
    {
        Obj obj;
        memset(&obj, 0, sizeof(obj));

	    _file.read((char *)&n, 4);
	    _file.read((char *)obj.name, n);


        uint16 num2;
	    _file.read((char *)&num2, 4);//num2
        char mb[4];
	    _file.read((char *)&mb, 4);

       	_file.read((char *)&n, 4);
        cout << dec << showbase << "vert " << n << endl;


        int init = _file.tellg();
        int vlen = 12;
        for(; vlen < 128; vlen+=4)
        {
            _file.seekg(init + vlen * n);

            uint32 facen;
	        _file.read((char *)&facen, 4);
            if(!_file.good())
                return;

            if(facen > 300 && facen < 3000)
                break;
        }
        cout << "vlen " << vlen << endl;

        cout << hex << showbase << " vb ";
        for(int t = 0; t < 2; t++)
        {
            uint32 ii = (unsigned char)vb[t];
            cout << ii << " ";
        }
        cout << " mb ";
        for(int t = 0; t < 2; t++)
        {
            uint32 ii = (unsigned char)mb[t];
            cout << ii << " ";
        }
        cout << dec << showbase << vlen;

        cout << endl;


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








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

	_file.read((char *)&ukn1, 4);//ukn1 0x2
	_file.read((char *)&ukn2, 4);//ukn2 0x1


    memset(&_name, 0, sizeof(_name));
	_file.read((char *)&n, 4);
	_file.read((char *)_name, n);

	_file.read((char *)&ukn3, 4);//ukn3[0,1]
	_file.read((char *)&num1, 4);//num1

    _file.seekg(28, ios::cur);//7f

    char sname[255];
	_file.read((char *)&n, 4);
    cout << "n1 " << n;
	_file.read((char *)&n, 4);
	_file.read((char *)sname, n);

	_file.read((char *)&num2, 4);//num2

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
    cout << " n2 " << n;
    //cout << "objs " << n << endl;
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
        //cout << dec << showbase << "vert " << vertn << endl;


        uint32 facen;
        int init = _file.tellg();
        int vlen = 12;
        for(; vlen < 128; vlen += 4)
        {
            _file.seekg(init + vlen * vertn);

	        _file.read((char *)&facen, 4);
            if(!_file.good())
                return;

            if(facen > vertn - 50 && facen < 999999)
            {
                //cout << "vlen " << vlen << endl;
                //cout << "facen " << facen << endl;

                for(int t = 0; t < facen; t++)
                {
                    uint16 idx;
	                _file.read((char *)&idx, 2);
                    if(!_file.good() || n > vertn)
                        return;
                }

                //int tt;
       	        //_file.read((char *)&tt, 4);
                //cout << "ssn " << tt << endl;
                //if(tt >= -1 && tt < 256)
                {
                    //cout << hex << showbase << " vb ";
                    //for(int t = 0; t < 4; t++)
                    //{
                    //    uint32 ii = (unsigned char)vb[t];
                    //    cout << ii << " ";
                    //}
                    //cout << " mb ";
                    //for(int t = 0; t < 4; t++)
                    //{
                    //    uint32 ii = (unsigned char)mb[t];
                    //    cout << ii << " ";
                    //}
                    //cout << "uuu" << ukn3 << " " <<  num1 << " " <<  num2 << " ";
                    cout << dec << showbase << " " << vlen;
                    cout << endl;
                    return;

                }
            }
        }

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








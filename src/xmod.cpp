#include "util.h"
#include "xmod.h"


Xmod::Xmod(const char* path)
{
	strcpy(_path, path);
	_file.open(_path, ios::in|ios::binary);

	//read head
	memset(&_head, 0, sizeof(_head));
	_file.seekg(0);
	_file.read((char *)&_head, sizeof(_head));

}


Xmod::~Xmod()
{
	_file.close();
}


void Xmod::info()
{
	cout << "=================================================" << endl;
	cout << "[Head]" << _path << endl;
    cout << "filetag    " << dec << showbase << _head.filetag << endl; 
    cout << "version    " << dec << showbase << _head.version << endl; 
    cout << "filecount  " << dec << showbase << (int)_head.filecount << endl; 
    cout << "tbloffset  " << hex << showbase << _head.tbloffset << endl;  
    cout << "encrypt    " << hex << showbase << _head.encrypt << endl; 
	cout << "=================================================" << endl;
    cout << dec << showbase << endl;
}

void Xmod::save()
{

}








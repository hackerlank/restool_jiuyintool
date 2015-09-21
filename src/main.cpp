#include <iostream>
#include <cmath>
#include "util.h"
#include "xmod.h"

using namespace std;

#define TestClass Xmod
#define TestFiles "res/npc/worldnpc029.xmod"

void work(const char *path)
{
	TestClass tt(path);
	tt.info();
}

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
	    for(int i = 1; i < argc; i ++)
	    	work(argv[i]);
    }
    else
    {
	    work(TestFiles);
    }

    return 0;
}



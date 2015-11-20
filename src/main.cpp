#include <iostream>
#include <cmath>
#include "util.h"
#include "xmod.h"

using namespace std;

#define TestClass Xmod
//#define TestFiles "res/npc/worldnpc029.xmod"
//#define TestFiles "res/girl/g_emei04.xmod"
//#define TestFiles "res/girl/TposeTmp.xmod"
//#define TestFiles "../res.package.files/res/obj/player/g_char/g_festival_1_7/g_festival_1_7.xmod"
//#define TestFiles "../res.package.files/res/obj/weapon/weapon_boss_003_exp/weapon_boss_003.xmod"
//#define TestFiles "../res.package.files/res/obj/weapon/sthorn/sthorn_006/sthorn_006.xmod"
//#define TestFiles "../char/g_fashion001/g_foots001.xmod"
#define TestFiles "../res.package.files/res/map/obj/effect/scenes_light_009.xmod"


void work(const char *path)
{
    cout << "[Read]" << path << endl;
	TestClass tt(path);
	//tt.info();
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



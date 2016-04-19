#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "scene.h"
#include "xml_parse_scene.h"
#include "render.h"

using namespace std;

int main()
{
    cout<< kuro::xml::parse::scene::parse_real("543.340") <<endl;

	system("pause");
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "scene.h"
#include "xml_parse_scene.h"
#include "render.h"

using namespace std;

int main(int argc, char** argv)
{
    std::string filename = "scene.xml";

	/*checks if an argument has been input*/
    if(argc == 2)
    {
        filename = argv[1];
    }

	/*parses the xml file into xml tags*/
    kuro::xml::tag::tag* tg = kuro::xml::parse::tags::parse(filename);

	/*parses the xml tags into a scene data structure*/
    kuro::scene::scene* scn = kuro::xml::parse::scene::parse(tg);

	/*creates the image which stores the rendered image data*/
    image::bmp img;
    img.resize(scn->settings.width, scn->settings.height);

	/*renders the scene and saves it*/
    render::render_block(0, 0, scn->settings.width-1, scn->settings.height-1, &img, scn);
	img.gamma_correction(1.0/2.5);
    img.save(scn->settings.name);

	system("pause");
    return 0;
}

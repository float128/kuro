#ifndef KURO_XML_UTIL_CPP
#define KURO_XML_UTIL_CPP

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <stdio.h>
#include "xml-data-structures.h"

std::string kuro::xml::util::read_file(std::string filename)
{
    FILE* file;
    std::string data;

    int c = 0;

    file = fopen(filename.c_str(), "r");

    if(file==NULL)return data;

    while(true)
    {
        c = getc (file);
        if(c == EOF)break;

        data += (char) c;
    }

    fclose(file);

    return data;
}

#endif

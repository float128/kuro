#ifndef KURO_XML_TAG_H
#define KURO_XML_TAG_H

#include "xml-data-structures.h"

namespace kuro
{
    namespace xml
    {
        namespace tag
        {
            /*stores an attribute name and value*/
            struct attribute
            {
                /*name of the attribute*/
                std::string name;

                /*value of the attribute*/
                std::string value;
            };

            /*stores a tag*/
            struct tag
            {
                /*name of the tag*/
                std::string name;

                /*a list of the attributes of the tag*/
                tag_attribute_list attribute_list;

                /*a pointer to the tag that comes after*/
                tag* next;

                /*a pointer to the first child tag*/
                tag* child;

                /*a pointer to the parent tag*/
                tag* parent;
            };
        }
    }
}

#endif

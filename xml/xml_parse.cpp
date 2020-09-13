#ifndef KURO_XML_PARSE_CPP
#define KURO_XML_PARSE_CPP

#include <iostream>

#include "xml-data-structures.h"
#include "xml_tag.h"
#include "xml_tokeniser_basic.h"
#include "xml_tokeniser_tags.h"


namespace kuro
{
    namespace xml
    {
        namespace parse
        {
            namespace tags
            {
                void convert_tag(tokeniser::tags::tag::tag* tkn_tag, tag::tag* tg)
                {
                    tg->name = tkn_tag->name;

                    tag::attribute attr;


                    for(int i = 0;i<tkn_tag->attribute_list.size();i++)
                    {
                        attr.name = tkn_tag->attribute_list[i].name;
                        attr.value = tkn_tag->attribute_list[i].value;

                        tg->attribute_list.push_back(attr);
                    }
                }

                void open_tag(tag::tag* tg, tag::tag* &parent, tag::tag* &prev)
                {
                    tg->parent = parent;
                    tg->child = NULL;
                    tg->next = NULL;

                    if(prev != NULL)
                    {
                        prev->next = tg;
                    }
                    else
                    {
                        parent->child = tg;
                    }

                    prev = NULL;
                    parent = tg;
                }

                void close_tag(tag::tag* &parent, tag::tag* &prev)
                {
                    prev = parent;
                    parent = parent->parent;
                }

                void continue_tag(tag::tag* tg, tag::tag* &parent, tag::tag* &prev)
                {
                    tg->parent = parent;
                    tg->child = NULL;
                    tg->next = NULL;

                    if(prev != NULL)
                    {
                        prev->next = tg;
                    }
                    else
                    {
                        if(parent != NULL)parent->child = tg;
                    }

                    prev = tg;
                }


                bool add_tag(tokeniser::tags::tag::tag* tg, tag::tag* &parent, tag::tag* &prev)
                {
                    if(tg->type == tokeniser::tags::tag::TAG_HEADER)
                    {
                        tag::tag* _tag = new tag::tag;
                        convert_tag(tg, _tag);

                        open_tag(_tag, parent, prev);

                        return true;
                    }

                    if(tg->type == tokeniser::tags::tag::TAG_TAIL)
                    {
                        close_tag(parent, prev);

                        return true;
                    }

                    if(tg->type == tokeniser::tags::tag::SELF_TAG)
                    {
                        tag::tag* _tag = new tag::tag;
                        convert_tag(tg, _tag);

                        continue_tag(_tag, parent, prev);

                        return true;
                    }

                    if(tg->type == tokeniser::tags::tag::PROLOGUE)
                    {
                        tag::tag* _tag = new tag::tag;
                        convert_tag(tg, _tag);

                        continue_tag(_tag, parent, prev);

                        return true;
                    }

                    if(tg->type == tokeniser::tags::tag::NULL_TAG)
                    {
                        return true;
                    }

                    return false;
                }

                tag::tag* parse(tokeniser::tags::tag::tag_list* list)
                {
                    tag::tag* prev_tag = NULL;
                    tag::tag* first_tag = NULL;
                    tag::tag* parent_tag = NULL;

                    for(int i = 0;i<list->size();i++)
                    {
                        bool no_error = add_tag(&(*list)[i], parent_tag, prev_tag);

                        if(i == 0)
                        {
                            first_tag = prev_tag;
                        }

                        if(!no_error)
                        {
                            std::cout<< "parse error occurred"<<std::endl;
                            break;
                        }
                    }

                    return first_tag;
                }

                tag::tag* parse(std::string filename)
                {
                    kuro::xml::tokeniser::basic::token::list tkn_list;
                    tkn_list = kuro::xml::tokeniser::basic::tokenise(filename);

                    kuro::xml::tokeniser::tags::tag::tag_list tag_list;
                    tag_list = kuro::xml::tokeniser::tags::tokenise(&tkn_list);

                    return kuro::xml::parse::tags::parse(&tag_list);
                }
            }
        }
    }
}

#endif

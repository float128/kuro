#ifndef KURO_XML_TOKENISER_BASIC_CPP
#define KURO_XML_TOKENISER_BASIC_CPP

#include <stdlib.h>
#include "xml-data-structures.h"
#include "xml_tokeniser_basic.h"

namespace kuro
{
    namespace xml
    {
        namespace tokeniser
        {
            namespace basic
            {
                bool scanner::subset(char c, std::string set)
                {
                    return (set.find(c) != std::string::npos);
                }

                bool scanner::tag::dummies::scan(const char* &file, token::list* list)
                {
                    while(true)
                    {
                        if(subset(file[0], charset::dummy))
                        {
                            file = &file[1];
                        }
                        else
                        {
                            break;
                        }
                    }

                    return true;
                }

                bool scanner::tag::open::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(scanner::subset(file[0], charset::dummy))
                    {
                        next = new scanner::tag::dummies;
                        next->next = new scanner::tag::open;
                        return true;
                    }

                    if(file[0] != '<')return false;

                    file++;

                    if(file[0] == '?')
                    {
                        next = new scanner::tag::open_qsn_mark;
                        return true;
                    }

                    if(file[0] == '/')
                    {
                        next = new scanner::tag::open_fwd_slash;
                        return true;
                    }

                    if(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        token::token tkn;
                        tkn.type = token::TAG_HEADER_OPEN;
                        tkn.value = "<";

                        list->push_back(tkn);

                        next = new scanner::tag::header_name;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::open_fwd_slash::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(file[0] != '/')return false;

                    file++;

                    token::token tkn;
                    tkn.type = token::TAG_TAIL_OPEN;
                    tkn.value = "</";

                    list->push_back(tkn);

                    if(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        next = new scanner::tag::tail_name;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::open_qsn_mark::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    token::token tkn;
                    tkn.value = "<?";
                    tkn.type = token::TAG_PROLOGUE_OPEN;
                    list->push_back(tkn);

                    file++;
                    if(file[0]=='x')
                    {
                        next = new scanner::tag::header_name;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::close::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(file[0] != '>')return false;

                    token::token tkn;
                    tkn.type = token::TAG_CLOSE;
                    tkn.value = ">";

                    list->push_back(tkn);

                    file++;
                    scanner::tag::dummies().scan(file, list);

                    if(file[0] == '<')
                    {
                        next = new scanner::tag::open;
                        return true;
                    }

                    if(file[0] == '\0')
                    {
                        next = NULL;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::close_fwd_slash::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(file[0] != '/')return false;
                    file++;
                    if(file[0] != '>')return false;
                    file++;

                    scanner::tag::dummies().scan(file, list);

                    token::token tkn;
                    tkn.type = token::SELF_TAG_CLOSE;
                    tkn.value = "/>";

                    list->push_back(tkn);

                    if(file[0] == '<')
                    {
                        next = new scanner::tag::open;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::close_qsn_mark::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(file[0] != '?')return false;

                    file++;
                    if(file[0] != '>')return false;

                    token::token tkn;
                    tkn.type = token::TAG_PROLOGUE_CLOSE;
                    tkn.value = "?>";

                    list->push_back(tkn);

                    file++;
                    scanner::tag::dummies().scan(file, list);

                    if(file[0] == '<')
                    {
                        next = new scanner::tag::open;
                        return true;
                    }

                    return false;
                }

                bool scanner::tag::header_name::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    token::token tkn;
                    tkn.value = "";
                    tkn.type = token::TAG_HEADER_NAME;

                    while(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        tkn.value += file[0];
                        file++;
                    }

                    if(tkn.value.size()>0)list->push_back(tkn);

                    scanner::tag::dummies().scan(file, list);

                    next = new scanner::attribute::split;
                    return true;
                }

                bool scanner::tag::tail_name::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    token::token tkn;
                    tkn.value = "";
                    tkn.type = token::TAG_TAIL_NAME;

                    while(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        tkn.value += file[0];
                        file++;
                    }

                    if(tkn.value.size()>0)list->push_back(tkn);

                    scanner::tag::dummies().scan(file, list);

                    if(file[0] == '>')
                    {
                        next = new scanner::tag::close;
                        return true;
                    }

                    return false;
                }


                bool scanner::attribute::name::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    scanner::tag::dummies().scan(file, list);

                    token::token tkn;
                    tkn.value = "";
                    tkn.type = token::ATTRIBUTE_NAME;

                    while(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        tkn.value += file[0];
                        file++;
                    }

                    if(tkn.value.size()>0)list->push_back(tkn);

                    scanner::tag::dummies().scan(file, list);

                    if(file[0] == '=')
                    {
                        next = new scanner::attribute::equal_sgn;
                        return true;
                    }

                    return false;
                }

                bool scanner::attribute::equal_sgn::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    file++;
                    scanner::tag::dummies().scan(file, list);

                    if((file[0] == '"')||(file[0] == '\''))
                    {
                        next = new scanner::attribute::value;
                        return true;
                    }

                    return false;
                }

                bool scanner::attribute::value::scan(const char* &file, token::list* list)
                {
                    next = NULL;
                    char end_char;

                    if(file[0] == '"')
                    {
                        end_char = '"';
                    }
                    else if(file[0] == '\'')
                    {
                        end_char = '\'';
                    }
                    else
                    {
                        return false;
                    }

                    file++;

                    token::token tkn;
                    tkn.type = token::ATTRIBUTE_VALUE;
                    tkn.value = "";

                    while((file[0] != end_char)&&(file[0] != '\0'))
                    {
                        tkn.value += file[0];
                        file++;
                    }

                    list->push_back(tkn);

                    if(file[0] == '\0')return false;

                    file++;
                    scanner::tag::dummies().scan(file, list);

                    next = new scanner::attribute::split;
                    return true;
                }

                bool scanner::attribute::split::scan(const char* &file, token::list* list)
                {
                    next = NULL;

                    if(file[0]=='?')
                    {
                        next = new scanner::tag::close_qsn_mark;
                        return true;
                    }

                    if(file[0]=='/')
                    {
                        next = new scanner::tag::close_fwd_slash;
                        return true;
                    }

                    if(file[0]=='>')
                    {
                        next = new scanner::tag::close;
                        return true;
                    }

                    if(scanner::subset(file[0], charset::alpha_numeric))
                    {
                        next = new scanner::attribute::name;
                        return true;
                    }

                    return false;
                }

                token::list tokenise(std::string filename)
                {
                    scanner::lex_scanner* scannr = NULL;
                    scanner::lex_scanner* next_scannr = new scanner::tag::open;

                    std::string xml_file_data = util::read_file(filename);

                    const char* file_data_ptr = xml_file_data.c_str();

                    token::list list;

                    for(scannr = next_scannr;
                    next_scannr!=NULL;
                    scannr=next_scannr)
                    {
                        bool error = !scannr->scan(file_data_ptr, &list);

                        if(error)
                        {
                            std::cout<< "#######...lexical error occurred...######"<<std::endl;
                            std::cout<< "#######...remaining characters not scanned...#######" <<std::endl;
                            std::cout<< file_data_ptr <<std::endl;
                            std::cout<< "####################################" <<std::endl;
                            exit(1);
                        }

                        next_scannr = scannr->next;
                        delete scannr;
                    }

                    delete scannr;

					token::token tkn;
					tkn.type = token::NULL_TAG;
					tkn.value = "";

					list.push_back(tkn);

                    return list;
                }

            }
        }
    }
}

#endif

#ifndef KURO_XML_TOKENSIER_TAGS_CPP
#define KURO_XML_TOKENISER_TAGS_CPP

#include "xml-data-structures.h"

#include "xml_tokeniser_basic.h"
#include "xml_tokeniser_tags.h"

namespace kuro
{
    namespace xml
    {
        namespace tokeniser
        {
			namespace tags
			{
				namespace scanner
				{
					namespace scan_tag
					{
						bool open::scan(basic::token::token* &tkn, tag::tag_list* list)
						{
                            basic::token::token_type type = tkn[0].type;
							tkn++;

							switch(type)
							{
								case basic::token::TAG_HEADER_OPEN:
									next = new header;
									return true;

								case basic::token::TAG_TAIL_OPEN:
									next = new tail;
									return true;

								case basic::token::TAG_PROLOGUE_OPEN:
									next = new prologue;
									return true;
							}

							next = NULL;
							return false;
						}

						bool header::scan(basic::token::token* &tkn, tag::tag_list* list)
						{
							next = NULL;

                            if(tkn[0].type != basic::token::TAG_HEADER_NAME)return false;

                            tag::tag tg;
                            tg.type = tag::NULL_TAG;
                            tg.name = tkn[0].value;

                            tkn++;
                            add_attributes(tkn, &tg);

                            if(tkn[0].type == basic::token::TAG_CLOSE)
                            {
                                tg.type = tag::TAG_HEADER;
                            }

                            if(tkn[0].type == basic::token::SELF_TAG_CLOSE)
                            {
                                tg.type = tag::SELF_TAG;
                            }

                            if(tg.type == tag::NULL_TAG)
                            {
                                return false;
                            }

                            tkn++;

                            list->push_back(tg);

                            next = new close;

                            return true;
						}

						bool tail::scan(basic::token::token* &tkn, tag::tag_list* list)
						{
							next = NULL;

                            if(tkn[0].type != basic::token::TAG_TAIL_NAME)return false;
                            if(tkn[1].type != basic::token::TAG_CLOSE)return false;

							tag::tag tg;
							tg.type = tag::TAG_TAIL;
							tg.name = tkn[0].value;
							list->push_back(tg);

							tkn++;
                            tkn++;

                            next = new close;

							return true;
						}

						bool prologue::scan(basic::token::token* &tkn, tag::tag_list* list)
						{
							next = NULL;

                            if(tkn[0].type != basic::token::TAG_HEADER_NAME)return false;

                            tag::tag tg;
                            tg.type = tag::PROLOGUE;
                            tg.name = tkn[0].value;

                            tkn++;
                            add_attributes(tkn, &tg);

                            if(tkn[0].type != basic::token::TAG_PROLOGUE_CLOSE)return false;
                            tkn++;

                            list->push_back(tg);

                            next = new close;

							return true;
						}

						bool close::scan(basic::token::token* &tkn, tag::tag_list* list)
						{
							next = NULL;

                            if(tkn[0].type == basic::token::NULL_TAG)return true;

                            next = new open;
							return true;
						}
					}

					void add_attributes(basic::token::token* &tkn, tag::tag* tg)
					{
                        while(true)
                        {
                            if(tkn[0].type != basic::token::ATTRIBUTE_NAME)
                            {
                                break;
                            }

                            if(tkn[1].type != basic::token::ATTRIBUTE_VALUE)
                            {
                                tkn++;
                                break;
                            }

                            tag::attribute attr;
                            attr.name = tkn[0].value;
                            attr.value = tkn[1].value;

                            tg->attribute_list.push_back(attr);

                            tkn++;
                            tkn++;
                        }
					}
				}


				tag::tag_list tokenise(basic::token::list* token_list)
				{
					scanner::tag_scanner* scannr = NULL;
                    scanner::tag_scanner* next_scannr = new scanner::scan_tag::open;

                    basic::token::token* tkn = &(*token_list)[0];

                    tag::tag_list list;

                    for(scannr = next_scannr;
                    next_scannr!=NULL;
                    scannr=next_scannr)
                    {
                        bool error = !scannr->scan(tkn, &list);

                        if(error)
                        {
                            std::cout<< "error occurred"<<std::endl;
                            break;
                        }

                        next_scannr = scannr->next;
                        delete scannr;
                    }

					tag::tag tg;
					tg.type = tag::NULL_TAG;

					list.push_back(tg);

                    delete scannr;
                    return list;
				}
			}
        }
    }
}

#endif

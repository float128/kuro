#ifndef KURO_XML_TOKENISER_TAGS_H
#define KURO_XML_TOKENISER_TAGS_H

#include "xml-data-structures.h"

namespace kuro
{
    namespace xml
    {
        namespace tokeniser
        {
			namespace tags
			{
				namespace tag
				{
				    /*stores a tag token*/
					struct tag
					{
					    /*stores the tag token type*/
						tag_type type;

						/*stores the name of the token*/
						std::string name;

                        /*stores a list of attributes*/
						tag_attribute_list attribute_list;
					};

                    /*stores a attribute name and its corresponding value*/
					struct attribute
					{
					    /*stores the name of the attribute*/
						std::string name;

						/*stores the value of the attribute*/
						std::string value;
					};
				}

				namespace scanner
				{
				    /*the base class for a tag token scanner*/
					class tag_scanner
					{
						public:
						    /*function is overloaded; the functions scans basic tokens and
						    adds tag tokens to the tag token list.*/
							virtual bool scan(basic::token::token* &tkn, tag::tag_list* list)
							{
								next = NULL;

								return false;
							};

                            /*stores the next tag token scanner*/
							tag_scanner* next;
					};

					namespace scan_tag
					{
						class open: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};

						class prologue: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};

						class header: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};

						class self: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};

						class tail: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};

						class close: public tag_scanner
						{
							public:
								bool scan(basic::token::token* &tkn, tag::tag_list* list);
						};


					}
				}
			}
        }
    }
}

#endif

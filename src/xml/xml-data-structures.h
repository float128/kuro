#ifndef KURO_XML_DATA_STRUCTURE_H
#define KURO_XML_DATA_STRUCTURE_H

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <string>

/*defines all data types*/
namespace kuro
{
    /*defines xml data types and functions*/
    namespace xml
    {
        /*defines token data types and function*/
        namespace tokeniser
        {
            /*produces the first set of tokens*/
            namespace basic
            {
                namespace token
                {
                    /*all the available token types*/
                    enum token_type{
                                TAG_HEADER_OPEN,
                                TAG_CLOSE,
                                SELF_TAG_CLOSE,
                                TAG_TAIL_OPEN,
                                TAG_PROLOGUE_OPEN,
                                TAG_PROLOGUE_CLOSE,
                                ATTRIBUTE_NAME,
                                ATTRIBUTE_VALUE,
                                TAG_HEADER_NAME,
                                TAG_TAIL_NAME,
								NULL_TAG
                            };
                    /*
                    TAG_HEADER_OPEN = <
                    TAG_CLOSE = >
                    SELF_TAG_CLOSE = />
                    TAG_TAIL_OPEN = </
                    TAG_PROLOGUE_OPEN = <?
                    TAG_PROLOGUE_CLOSE = ?>
                    ATTRIBUTE_NAME = (?=\s+)[a-zA-Z_0-9]+(?=\s+)
                    ATTRIBUTE_VALUE = (?=")[^"]+(?=")
                    TAG_HEADER_NAME = (?=(TAG_HEADER_OPEN|TAG_PROLOGUE_OPEN))[a-zA-Z_0-9]+(?=\s)
                    TAG_TAIL_NAME = (?=TAG_TAIL_OPEN)[a-zA-Z_0-9]+(?=TAG_CLOSE)
                    NULL_TAG = ""
                    */

                    /*structure is used to store a single token*/
                    struct token;

                    /*stores a dynamic list of tokens*/
                    typedef std::vector <token> list;
                }

                /*scans characters from file and converts into tokens*/
                namespace scanner
                {
                    namespace charset
                    {
                        /*string contains a sequence of all alpha numeric characters*/
                        static std::string alpha_numeric =  "abcdefghijklmnopqrstuvwxyz"
                                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                            "0123456789_";

                        /*string contains a sequence of characters to be ignored*/
                        static std::string dummy = "\n\r\t ";
                    }

                    /*the base class of lexical scanners*/
                    class lex_scanner;

                    /*stores scanners for different tag related lexemes*/
                    namespace tag
                    {
                        /*scans characters to be ignored*/
                        class dummies;

                        /*scans for open tags*/
                        class open;

                        /*scans for a forward slash after a less than character*/
                        class open_fwd_slash;

                        /*scans for a question mark after a less than character*/
                        class open_qsn_mark;

                        /*scans for a greater than character at the closing of a tag*/
                        class close;

                        /*scans for a forward slash at the closing of a tag*/
                        class close_fwd_slash;

                        /*scans for a question mark at the closing of a prologue tag*/
                        class close_qsn_mark;

                        /*scans for a header name for a tag*/
                        class header_name;

                        /*scans for a name for a closing tag*/
                        class tail_name;
                    }

                    /*stores scanners for different attribute related lexemes*/
                    namespace attribute
                    {
                        /*scans for an attribute name*/
                        class name;

                        /*scans for an equals sign*/
                        class equal_sgn;

                        /*scans for an attribute value*/
                        class value;

                        /*scans for the split between different attribute name and value pairs*/
                        class split;
                    }

                    /*returns true if the character "c" is within the "set" string*/
                    bool subset(char c, std::string set);
                }

                /*the input file is scanned for tokens which are stored in the dynamic
                array returned*/
                token::list tokenise(std::string filename);
            }

            /*produces tag tokens*/
			namespace tags
			{
                /*stores tag data types*/
				namespace tag
				{
				    /*stores the type of tag token*/
					enum tag_type{TAG_HEADER, TAG_TAIL, SELF_TAG, PROLOGUE, NULL_TAG};

					/*
					TAG_HEADER - <name attribute_1="value_1" attribute_2="value_2" ... attribute_n="value_n">
                    TAG_TAIL - </name>
                    SELF_TAG - <name attribute_1="value_1" attribute_2="value_2" ... attribute_n="value_n"/>
                    PROLOGUE - <?xml attribute_1="value_1" attribute_2="value_2" ... attribute_n="value_n"?>
                    NULL_TAG - last tag in token array
					*/

                    /*structure is used to store a tag token*/
					struct tag;

                    /*structure is used to store a attribute name and value*/
                    struct attribute;

                    /*stores a dynamic array of tag tokens*/
					typedef std::vector <tag> tag_list;

					/*stores a dynamic array of attribute name, value pairs*/
					typedef std::vector <attribute> tag_attribute_list;
				}

                /*scans for tag tokens from an array of the initial tokens produced*/
				namespace scanner
				{
                    /*base class for tag scanners*/
					class tag_scanner;

                    /*scanners for different tag token types*/
					namespace scan_tag
					{
					    /*scans for the opening of a tag*/
						class open;

						/*scans for a prologue tag*/
						class prologue;

						/*scans for a header tag*/
						class header;

						/*scans for a self closing tag*/
						class self;

						/*scans for a closing of a non self-closing tag*/
						class tail;

						/*scans for the closing of a tag*/
						class close;
					}

                    /*scans for attributes and adds them to a tag token*/
                    void add_attributes(basic::token::token* &tkn, tag::tag* tg);
				}

                /*produces tag tokens from an array of initial tokens*/
				tag::tag_list tokenise(basic::token::list* list);
			}
        }

        namespace util
        {
            /*reads a file and converts to a string*/
            std::string read_file(std::string filename);
        }

        namespace tag
        {
            /*stores a tag data structure*/
            struct tag;

            /*stores an attribute name and value*/
            struct attribute;

            /*stores a dynamic array of attribute name, value pairs*/
            typedef std::vector <attribute> tag_attribute_list;
        }

        /*parsing tag tokens into tag data structures linked by pointers*/
        namespace parse
        {
            namespace tags
            {
                /*converts a tag token into a tag data structure*/
                void convert_tag(tokeniser::tags::tag::tag* tkn_tag, tag::tag* tg);

                /*applies the parent child relationships between tags,
                the parent pointer is updated to point to a new tag*/
                void open_tag(tag::tag* tg, tag::tag* &parent, tag::tag* &prev);

                /*applied the parent child relationships between tags,
                the parent pointer is updated to point to the parent of the
                of the parent pointer*/
                void close_tag(tag::tag* &parent, tag::tag* &prev);

                /*the parent pointer remains unchanged*/
                void continue_tag(tag::tag* tg, tag::tag* &parent, tag::tag* &prev);

                /*creates a new tag from a tag token*/
                bool add_tag(tokeniser::tags::tag::tag* tg, tag::tag* &parent, tag::tag* &prev);

                /*creates tags from a list of tag tokens*/
                tag::tag* parse(tokeniser::tags::tag::tag_list* list);

                /*creates tags from a file*/
                tag::tag* parse(std::string filename);
            }
        }
    }
}

#endif

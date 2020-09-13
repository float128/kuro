#ifndef KURO_XML_TOKENISER_BASIC_H
#define KURO_XML_TOKENISER_BASIC_H

#include <iostream>

#include "xml-data-structures.h"

namespace kuro
{
    namespace xml
    {
        namespace tokeniser
        {
            namespace basic
            {
                namespace token
                {
                    /*stores a token*/
                    struct token
                    {
                        /*stores the name of the token*/
                        std::string value;

                        /*stores the type of tag token*/
                        token_type type;
                    };
                }

                namespace scanner
                {
                    /*the basic lexical scanner base class*/
					class lex_scanner
                    {
                        public:
                            /*the function is overloaded; the functions scans characters and
                            adds tokens to the token list*/
                            virtual bool scan(const char* &file, token::list* list)
                            {
                                next = NULL;
                                std::cout<<"error"<<std::endl;
                                return false;
                            };

                            /*stores the next scanner*/
                            lex_scanner* next;
                    };

                    namespace tag
                    {
                        class dummies: public lex_scanner
                        {
                            public:
                                    bool scan(const char* &file, token::list* list);
                        };


                        class open: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class open_fwd_slash: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class open_qsn_mark: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class close: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class close_fwd_slash: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class close_qsn_mark: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class header_name: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class tail_name: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };
                    }

                    namespace attribute
                    {
                        class name: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class equal_sgn: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class value: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };

                        class split: public lex_scanner
                        {
                            public:
                                bool scan(const char* &file, token::list* list);
                        };
                    }
                }
            }
        }
    }
}


#endif

/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : symbols_table.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef symbols_table_h
#define symbols_table_h

#include <string>
#include <map>
#include <vector>
#include <stdio.h>

#include "tokens.h"

using namespace std;

void ts_tests(void);

struct c_token
{
    int     id;
    string  text;

    c_token()
    {
        id = 0;
        text  = "";
    }

    c_token(int t, char * s)
    {
        id = t;
        text  = s;
    }
};

/*
  when a token is analised it can become to symbols_table

  class A

  class -> does not become a symbol
  A -> become a symbol
*/
struct base_class
{
  string text;
  int     access_specifier;
};
typedef map<string, base_class> map_base_class;

struct c_symbol
{
  c_token token;
  int type;
  int class_key;

  c_symbol()
  {
    type  = 0;
    class_key = 0;
  }

  c_symbol(c_token token_1)
  {
      token = token_1;
      type  = 0;
      class_key = 0;
  }

  void print(const char * tab)
  {
      printf("%sid[%d]->[%s] text[%s] type[%d]->[%s] class_key[%d]->[%s]\n"
        , tab
        , token.id
        , yytokens[token.id]
        , token.text.c_str()
        , type
        , yytokens[type]
        , class_key
        , yytokens[class_key]
      );
  }
};

typedef map<string, c_symbol> t_symbols;
typedef vector<t_symbols> stack_symbols;

class c_symbols_table
{
  private:
    stack_symbols stack;

  public:
    c_symbols_table();
    ~c_symbols_table();
    void set();
    void unset();

    void insert(c_symbol symbol);
    void print(void);

    c_symbol search_symbol(string str);
};

extern c_symbols_table ts;

#endif
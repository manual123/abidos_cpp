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
#include "lex_yacc.h"


using namespace std;

void ts_tests(void);

struct c_token
{
  int id;
  string text;

  string file;
  int line;

  c_token()
  {
    id = 0;
    text = "";

    file = "";
    line = 0;
  } c_token(int t, char *s)
  {
    id = t;
    text = s;

    file = lex_file_name;
    line = yylineno;
  }
  void clear(void)
  {
    id = 0;
    text = "";

    file = "";
    line = 0;
  }
};

/*
 * when a token is analised it can become to symbols_table
 *
 * class A
 *
 * class -> does not become a symbol A -> become a symbol
 */
struct c_base_class
{
  string text;
  int access_specifier;

  c_base_class()
  {
    text = "";
    access_specifier = 0;
  } c_base_class(string t, int access)
  {
    text = t;
    access_specifier = access;
  }
};

typedef map < string, c_base_class > t_map_base_class;
typedef vector < c_base_class > t_vector_base_class;



/*
 * decl_specifier: storage_class_specifier | type_specifier |
 * function_specifier | FRIEND | TYPEDEF ;
 *
 * ## todo vector<c_decl_specifier> t_vector_decl_specifier
 */
struct c_decl_specifier
{
  c_token token;

  int storage_class_specifier;
  int type_specifier;
  int function_specifier;
  int friend_specifier;
  int typedef_specifier;

  c_decl_specifier(c_token t)
  {
    token = t;

    storage_class_specifier = 0;
    type_specifier = 0;
    function_specifier = 0;
    friend_specifier = 0;
    typedef_specifier = 0;
  } void clear(void)
  {
    storage_class_specifier = 0;
    type_specifier = 0;
    function_specifier = 0;
    friend_specifier = 0;
    typedef_specifier = 0;
  }
};

typedef vector < c_decl_specifier > t_vector_decl_specifier;

struct c_parameter
{
  t_vector_decl_specifier vector_decl_specifier;

  c_token token;

  c_parameter()
  {

  } c_parameter(c_token t, t_vector_decl_specifier v)
  {
    token = t;
    vector_decl_specifier = v;
  }
  void print(const char *tab);
  string get_string(void);
};

typedef vector < c_parameter > t_vector_parameter;
typedef map < string, c_parameter > t_map_parameter;

class c_declarator
{
protected:
  t_map_parameter map_parameter;
  t_vector_parameter vector_parameter;
public:

  c_token token;
  t_vector_decl_specifier vector_decl_specifier;

  int is_function;
  void parameter_insert(c_parameter parameter);
  void print(const char *tab);

  c_declarator()
  {
    is_function = 0;
  }
  c_declarator(c_token t, t_vector_decl_specifier v)
  {
    token = t;
    vector_decl_specifier = v;

    is_function = 0;
  }
  void clear(void)
  {
    map_parameter.clear();
    vector_parameter.clear();
    token.clear();
    is_function = 0;
  }
  string get_full_name(void);
};


/*
 * class A { long int a; }
 *
 * c_class_member has 'long int a' inside
 *
 * if is a function member can have parameters
 */

class c_class_member: public c_declarator
{
public:

  int access_specifier;

  c_class_member()
  {
//    access_specifier = 0;
  }
  c_class_member(c_token t, t_vector_decl_specifier v):c_declarator(t, v)
  {
//    access_specifier = 0;
  }
};

typedef map < string, c_class_member > t_map_class_member;
typedef vector < c_class_member * >t_vector_class_member;

/*
 * i store members in map to have fast access i store members in vector to
 * haver the original order
 */
class c_class_members
{
private:
  t_map_class_member map_class_member;
  t_vector_class_member vector_class_member;
  friend class c_generator_class_diagram;
  friend class c_generator_original;
public:
  void clear(void);
  void print(const char *tab);
  void insert(c_class_member member);
  c_class_member *get(string member);
};

class c_symbol
{
private:
  t_map_base_class map_base_class;
  t_vector_base_class vector_base_class;
  friend class c_generator_class_diagram;
  friend class c_generator_original;

public:
  void insert(c_base_class base_class)
  {
    map_base_class[base_class.text] = base_class;
    vector_base_class.push_back(base_class);
  }

  c_token token;
  int type;
  int class_key;

  // t_map_class_member map_class_member;
  c_class_members members;

  int free_declarator;
  c_declarator declarator;

  c_symbol()
  {
    type = 0;
    class_key = 0;
    map_base_class.clear();
    members.clear();

    free_declarator = 0;
  }

  c_symbol(c_token token_1)
  {
    token = token_1;
    type = 0;
    class_key = 0;
    map_base_class.clear();
    members.clear();

    free_declarator = 0;
    declarator.clear();
  }

  ~c_symbol()
  {
    type = 0;
    class_key = 0;
    map_base_class.clear();
    members.clear();

    free_declarator = 0;
    declarator.clear();
  }
  void print(const char *tab);
};

typedef map < string, c_symbol > t_symbols;
typedef vector < t_symbols > stack_symbols;

class c_symbols_table
{
private:
  stack_symbols stack;
  friend class c_generator_class_diagram;
  friend class c_generator_original;
public:
  c_symbols_table();
  ~c_symbols_table();
  void set();
  void unset();

  void insert(c_symbol symbol);
  void print(void);

  c_symbol *search_symbol(string str);
};

extern c_symbols_table ts;

#endif

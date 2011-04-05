/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_class_diagram.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 05/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdio.h>
#include "generator_class_diagram.h"
#include "lex_yacc.h"
/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::members_url(t_vector_class_member &	vector_class_member)
{
  unsigned i_member = 0;
  for(i_member = 0; i_member < vector_class_member.size(); ++i_member)
  {
		fprintf(f_out,"%s;",vector_class_member[i_member]->token.text.c_str());
  }
}

/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::members_label(t_vector_class_member &	vector_class_member)
{
  unsigned i_member = 0;
  for(i_member = 0; i_member < vector_class_member.size(); ++i_member)
  {
		fprintf(f_out,"%s\\l",vector_class_member[i_member]->token.text.c_str());
		fprintf(f_out,"\\l");
  }
}
/*----------------------------------------------------------------------------*/
/*
        A [
          URL="A;a1;age;die",
          label = "{A|+ a1 : string\l+ age : int\l|+ die() : void\l}"
        ]
*/
void c_generator_class_diagram::classes(c_symbol & symbol)
{
//first[B]    id[258]->[IDENTIFIER] text[B] type[265]->[CLASS_NAME] class_key[300]->[CLASS]
	if( CLASS_NAME != symbol.type)
	{
		return;
	}
	fprintf(f_out,"  %s [\n",symbol.token.text.c_str() );
	fprintf(f_out,"    URL=\"%s;", symbol.token.text.c_str());
	members_url(symbol.members.vector_class_member);
	fprintf(f_out,"\",\n");

	fprintf(f_out,"    label=\"{%s|", symbol.token.text.c_str());
	members_label(symbol.members.vector_class_member);
	fprintf(f_out,"}\"\n");
	fprintf(f_out,"  ]\n");
	
}
/*----------------------------------------------------------------------------*/
/*
     B->Animal;
*/
void c_generator_class_diagram::inheritance(c_symbol & symbol)
{
	if( CLASS_NAME != symbol.type)
	{
		return;
	}	

  t_map_base_class::iterator i_map_base = symbol.map_base_class.begin();

  for( i_map_base = symbol.map_base_class.begin(); i_map_base != symbol.map_base_class.end(); ++i_map_base )
  {
  	fprintf(f_out,"  %s->%s;\n"
  	, symbol.token.text.c_str()
  	, ((*i_map_base).second).text.c_str()
  	);
  }
}
/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::run(char * p_file_out)
/*
	## todo this is a print copied from ts::print, 
	   change this code to generate graphviz diagram
*/
{
  printf("## void c_generator_class_diagram::run(char * p_file_out [%s])\n",p_file_out);

	f_out = NULL;

	f_out = fopen("out.gv","w");
	if( NULL == f_out )
	{
		printf("  c_generator_class_diagram::run() cant fopen()\n");
		return;
	}

//	fprintf(f_out,"digraph defines {\n");
//	fprintf(f_out,"  graph [ranksep=0];\n");<
  

  unsigned i_stack = 0;

  t_symbols::iterator i_map;

  printf("##{\n");
  for( i_stack = 0; i_stack < ts.stack.size(); ++i_stack )
  {
    printf("  stack level[%d]\n",i_stack);
    for( i_map = ts.stack[i_stack].begin(); i_map != ts.stack[i_stack].end(); ++i_map )
    {
    	classes( (*i_map).second );
    }
  }
  printf("}\n");
  
  
  printf("##{\n");
  for( i_stack = 0; i_stack < ts.stack.size(); ++i_stack )
  {
    printf("  stack level[%d]\n",i_stack);
    for( i_map = ts.stack[i_stack].begin(); i_map != ts.stack[i_stack].end(); ++i_map )
    {
    	inheritance( (*i_map).second );
    }
  }
  printf("}\n");  
  
  if( NULL != f_out )
  {
  	fclose(f_out);
  	f_out = NULL;
  }
}
/*----------------------------------------------------------------------------*/

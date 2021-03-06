/*	$Id: lexer.l,v 1.2 1997/11/19 15:13:15 sandro Exp $	*/

/*
 * Copyright (c) 1997 Sandro Sigala <ssigala@globalnet.it>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * ISO C++ lexical analyzer.
 *
 * Based on the ISO C++ draft standard of December '96.
 */

%{
#include <ctype.h>
#include <stdio.h>

#include "parser.h"
#define LONG_STR (4000)

//static int yywrap(void);
char lex_file_name[LONG_STR];

static void skip_until_eol(void);
static void skip_comment(void);
static int check_identifier(const char *);
// i chosed the elsa string works better than this
//void get_string(void);
%}

intsuffix				([uU][lL]?)|([lL][uU]?)
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
chartext				([^'])|(\\.)
stringtext				([^"])|(\\.)
/*
  from elsa
  http://scottmcpeak.com/elkhound/sources/elsa/cc.lex
  {
*/

/* newline */
NL            "\n"
/* anything but newline */
NOTNL         .
/* any of 256 source characters */
ANY           ({NOTNL}|{NL})
/* backslash */
BACKSL        "\\"
QUOTE         [\"]
/* normal string character: any but quote, newline, or backslash */
STRCHAR       [^\"\n\\]
/* (start of) an escape sequence */
ESCAPE        ({BACKSL}{ANY})
/*
}
*/

%option noyywrap
%option yylineno
%%

"\n"					{ }
[\t\f\v\r ]+				{ /* Ignore whitespace. */ }

"/*"					{ skip_comment(); }
"//ban_symbols_on" { return ABIDOS_BAN_SYMBOLS_ON; }
"//ban_symbols_off" { return ABIDOS_BAN_SYMBOLS_OFF; }
"//"					{ skip_until_eol(); }
"#"					{ return '#'; }
"{"					{ return '{'; }
"<%"					{ return '{'; }
"}"					{ return '}'; }
"%>"					{ return '}'; }
"["					{ return '['; }
"<:"					{ return '['; }
"]"					{ return ']'; }
":>"					{ return ']'; }
"("					{ return '('; }
")"					{ return ')'; }
";"					{ return ';'; }
":"					{ return ':'; }
"..."					{ return ELLIPSIS; }
"?"					{ return '?'; }
"::"					{ return COLONCOLON; }
"."					{ return '.'; }
".*"					{ return DOTSTAR; }
"+"					{ return '+'; }
"-"					{ return '-'; }
"*"					{ return '*'; }
"/"					{ return '/'; }
"%"					{ return '%'; }
"^"					{ return '^'; }
"xor"					{ return '^'; }
"&"					{ return '&'; }
"bitand"				{ return '&'; }
"|"					{ return '|'; }
"bitor"					{ return '|'; }
"~"					{ return '~'; }
"compl"					{ return '~'; }
"!"					{ return '!'; }
"not"					{ return '!'; }
"="					{ return '='; }
"<"					{ return '<'; }
">"					{ return '>'; }
"+="					{ return ADDEQ; }
"-="					{ return SUBEQ; }
"*="					{ return MULEQ; }
"/="					{ return DIVEQ; }
"%="					{ return MODEQ; }
"^="					{ return XOREQ; }
"xor_eq"				{ return XOREQ; }
"&="					{ return ANDEQ; }
"and_eq"				{ return ANDEQ; }
"|="					{ return OREQ; }
"or_eq"					{ return OREQ; }
"<<"					{ return SL; }
">>"					{ return SR; }
"<<="					{ return SLEQ; }
">>="					{ return SREQ; }
"=="					{ return EQ; }
"!="					{ return NOTEQ; }
"not_eq"				{ return NOTEQ; }
"<="					{ return LTEQ; }
">="					{ return GTEQ; }
"&&"					{ return ANDAND; }
"and"					{ return ANDAND; }
"||"					{ return OROR; }
"or"					{ return OROR; }
"++"					{ return PLUSPLUS; }
"--"					{ return MINUSMINUS; }
","					{ return ','; }
"->*"					{ return ARROWSTAR; }
"->"					{ return ARROW; }

"asm"					{ return ASM; }
"auto"					{ return AUTO; }
"bool"					{ return BOOL; }
"break"					{ return BREAK; }
"case"					{ return CASE; }
"catch"					{ return CATCH; }
"char"					{ return CHAR; }
"class"					{ return CLASS; }
"const"					{ return CONST; }
"const_cast"				{ return CONST_CAST; }
"continue"				{ return CONTINUE; }
"default"				{ return DEFAULT; }
"delete"				{ return DELETE; }
"do"					{ return DO; }
"double"				{ return DOUBLE; }
"dynamic_cast"				{ return DYNAMIC_CAST; }
"else"					{ return ELSE; }
"enum"					{ return ENUM; }
"explicit"				{ return EXPLICIT; }
"export"				{ return EXPORT; }
"extern"				{ return EXTERN; }
"false"					{ return FALSE; }
"float"					{ return FLOAT; }
"for"					{ return FOR; }
"friend"				{ return FRIEND; }
"goto"					{ return GOTO; }
"if"					{ return IF; }
"inline"				{ return INLINE; }
"int"					{ return INT; }
"long"					{ return LONG; }
"mutable"				{ return MUTABLE; }
"namespace"				{ return NAMESPACE; }
"new"					{ return NEW; }
"operator"				{ return OPERATOR; }
"private"				{ return PRIVATE; }
"protected"				{ return PROTECTED; }
"public"				{ return PUBLIC; }
"register"				{ return REGISTER; }
"reinterpret_cast"			{ return REINTERPRET_CAST; }
"return"				{ return RETURN; }
"short"					{ return SHORT; }
"signed"				{ return SIGNED; }
"sizeof"				{ return SIZEOF; }
"static"				{ return STATIC; }
"static_cast"				{ return STATIC_CAST; }
"struct"				{ return STRUCT; }
"switch"				{ return SWITCH; }
"template"				{ return TEMPLATE; }
"this"					{ return THIS; }
"throw"					{ return THROW; }
"true"					{ return TRUE; }
"try"					{ return TRY; }
"typedef"				{ return TYPEDEF; }
"typeid"				{ return TYPEID; }
"typename"				{ return TYPENAME; }
"union"					{ return UNION; }
"unsigned"				{ return UNSIGNED; }
"using"					{ return USING; }
"virtual"				{ return VIRTUAL; }
"void"					{ return VOID; }
"volatile"				{ return VOLATILE; }
"wchar_t"				{ return WCHAR_T; }
"while"					{ return WHILE; }

[a-zA-Z_][a-zA-Z_0-9]*			{ return check_identifier(yytext); }

"0"[xX][0-9a-fA-F]+{intsuffix}?		{ return INTEGER; }
"0"[0-7]+{intsuffix}?			{ return INTEGER; }
[0-9]+{intsuffix}?			{ return INTEGER; }

{fracconst}{exppart}?{floatsuffix}?	{ return FLOATING; }
[0-9]+{exppart}{floatsuffix}?		{ return FLOATING; }

"'"{chartext}*"'"			{ return CHARACTER; }
"L'"{chartext}*"'"			{ return CHARACTER; }

  /* string literal from elsa*/
"L"?{QUOTE}({STRCHAR}|{ESCAPE})*{QUOTE} {
  return STRING;
}

. {
  fprintf(stderr, "lexx errro line[%d]: unexpected character [%c]\n", yylineno, yytext[0]);
  fprintf(stdout, "lexx errro line[%d]: unexpected character [%c]\n", yylineno, yytext[0]);
  return yytext[0];
}

%%
/*

#### just in sustitution get_string...
"\""{stringtext}*"\""			{ return STRING; }
"L\""{stringtext}*"\""			{ return STRING; }
*/
/*
static int
yywrap(void)
{
	return 1;
}
*/

/*
 * We use this routine instead a lex pattern because we don't need
 * to save the matched comment in the `yytext' buffer.
 */
static void
skip_comment(void)
{
	int c1, c2;

	c1 = input();
	c2 = input();

	while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
		c1 = c2;
		c2 = input();
	}
}
/*
void get_string(void)
{
	int c1, c2;
	int n = 0;
  yytext[n] = '"';

	c1 = input();
	c2 = input();

  ++n;
  yytext[n] = c1;

	while (c2 != EOF && !(c1 == '\\' && c2 == '"')) {
	  ++n;
	  yytext[n] = c2;
		c1 = c2;
		printf("####: mark_77a[%c]\n",c1);
		c2 = input();
	}

  ++n;
	yytext[n] = '"';

  ++n;
	yytext[n] = '\0';

  printf("####: mark_77b[%s]\n",yytext);
}
*/
/*
 * See the previous comment for an explanation.
 */
void
skip_until_eol(void)
{
	int c;

	while ((c = input()) != EOF && c != '\n')
		;
}

void
skip_until_eol_preprocessor(void)
{
	int c;
	while ((c = input()) != EOF && c != '\n')
	{
    // the funny '\n' can be for this lands...
	  if( '\'' == c ) {
      while ((c = input()) != EOF && c != '\'');
	  }

	  if( '\\' == c ) {
      skip_until_eol();
	  }
  }
}

static int
check_identifier(const char *s)
{
	/*
	 * This function should check if `s' is a typedef name or a class
	 * name, or a enum name, ... etc. or an identifier.
	 */
/*
	switch (s[0]) {
	case 'D': return TYPEDEF_NAME;
	case 'N': return NAMESPACE_NAME;
	case 'C': return CLASS_NAME;
	case 'E': return ENUM_NAME;
	case 'T': return TEMPLATE_NAME;
	}
*/
/* this is doing in parser_descent token_next()
*/
	return IDENTIFIER;
}

void get_string_between_sharps(char * str)
{
	int c;
	int n = 0;

	while ((c = input()) != EOF && c != '>'){
	  str[n] = c;
	  n++;
	}

  str[n] = '\0';
}

#define STACK_FILE_NAME_LEN (1024)

struct s_stack
{
  char * name;
  int lineno;
};

struct s_stack * stack_file_name[STACK_FILE_NAME_LEN];
int stack_top = 0;

void stack_file_name_init(void)
{
  int i = 0;

  for(i = 0; i < STACK_FILE_NAME_LEN; ++i){
    stack_file_name[STACK_FILE_NAME_LEN] = NULL;
  }

  stack_top = 0;
}

void stack_file_name_push(void)
{
  if ( STACK_FILE_NAME_LEN == stack_top )
  {
    printf("     error stack_file_name_push overflow[%d]\n", stack_top);
    exit(-1);
  }

  stack_file_name[stack_top] =
    (struct s_stack *) malloc(sizeof(struct s_stack) );

  stack_file_name[stack_top]->name =
    (char *) malloc(sizeof(char) * (strlen(lex_file_name) + 1));

  stack_file_name[stack_top]->lineno = yylineno;
  sprintf(stack_file_name[stack_top]->name,"%s",lex_file_name);

  ++stack_top;
}

void stack_file_name_pop(void)
{
  if ( 0 == stack_top )
  {
    printf("     error stack_file_name_pop downflow[%d]\n", stack_top);
    exit(-1);
  }

  --stack_top;
  yyset_lineno(stack_file_name[stack_top]->lineno);
  sprintf(lex_file_name,"%s",stack_file_name[stack_top]->name);

  free(stack_file_name[stack_top]->name);
  stack_file_name[stack_top]->name = NULL;
  stack_file_name[stack_top]->lineno = 0;
  free(stack_file_name[stack_top]);

  stack_file_name[stack_top] == NULL;

// lexer.l: warning: statement with no effect
//  fclose(yyin);
}

int lex_file_init(const char *f)
{
    stack_file_name_init();

    sprintf(lex_file_name,"%s",f);
    
//    printf("## [%s]\n",f);
    yyin=fopen(f,"r");
    if( NULL==yyin )
    {
        printf("     error lex_file_init open [%s]\n",f);
        return 0;
    }

    return 1;
}

int lex_file_exists(const char *f)
{
    unsigned len = strlen(f);
    unsigned i = 0;
    char file_name[LONG_STR];
    FILE *f_to_check = NULL;

    if( f[0] == '"' ){
      file_name[i] = f[i+1];
      ++i;
      while( i < len ){
        if( '"' == f[i+1]){
          file_name[i] = '\0';
          break;
        }

        file_name[i] = f[i+1];
        ++i;
      }
    }else{
      sprintf(file_name,"%s",f);
    }

    f_to_check = fopen(file_name,"r");
    if( NULL == f_to_check )
    {
        printf("     error lex_file_push open [%s]\n", file_name);
        return 0;
    }

    fclose(f_to_check);
    return 1;
}

int lex_file_push(const char *f)
{
    unsigned len = strlen(f);
    unsigned i = 0;
//##
//      printf("##:    lex_file_push [%s]\n",f);

    if(len < 3){
      printf("     error lex_file_push file too short [%s]\n",f);
    }

    if(1 != lex_file_exists(f) ){
      return 0;
    }

    //save the actual file name
    stack_file_name_push();
    yyset_lineno(1);

    if( f[0] == '"' ){    
      lex_file_name[i] = f[i+1];
      ++i;
      while( i < len ){
        if( '"' == f[i+1]){
          lex_file_name[i] = '\0';
          break;
        }

        lex_file_name[i] = f[i+1];
        ++i;
      }
    }else{
      sprintf(lex_file_name,"%s",f);
    }

//    printf("## [%s]\n",lex_file_name);
    yyin=fopen(lex_file_name,"r");
    if( NULL==yyin )
    {
        printf("     error lex_file_push open [%s]\n",lex_file_name);
        return 0;
    }

    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

//    yy_init_globals();
    return 1;
}

int lex_file_pop(void)
{
  stack_file_name_pop();
  yypop_buffer_state();

  if ( !YY_CURRENT_BUFFER ){
    yyterminate();
  }
//  printf("## [%s]\n",lex_file_name);
  return 1;
}

int lex_stack_size(void)
{
  return stack_top;
}

void lex_file_end(void)
{
    fclose(yyin);
}


/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : parser_descent.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef parser_descent_h
#define parser_descent_h

#include "lex_yacc.h"

#include <string>
#include <vector>

using namespace std;

#define NO_IDENTIFIER "@IDENTIFIER#"
#define NO_CLASS_NAME "NO_CLASS_NAME#"

#include "symbols_table.h"
#include "trace.h"

int chain_is_tail(string class_name_declaration, char * text);

typedef vector < c_token > t_tokens;

/*
 * tokens_vector =[CLASS A { } ;] when we are parsing A context will have
 * class_key == CLASS to know what is a
 */
enum t_class_specifier_status {
    NO_CLASS_STATUS = 0,
    CLASS_SPECIFIER_STATUS_IDENTIFIER,
    CLASS_SPECIFIER_STATUS_BASE_DECLARATION,
    CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER,
    CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR,
    CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR
};

static const char *const table_parser_status[] = {
    "NO_CLASS_STATUS",
    "CLASS_SPECIFIER_STATUS_IDENTIFIER",
    "CLASS_SPECIFIER_STATUS_BASE_DECLARATION",
    "CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER",
    "CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR",
    "CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR",
    0
};

/*
 * if you adde more members pls actualize restore_but_not_i_token()
 */
struct c_context {
    unsigned i_token;
    enum t_class_specifier_status class_specifier_status;	// the
    // semantic
    // will
    // know is
    // inside
    // a
    // class/struct/union
    // declaration

    int class_key;		// the rule when CLASS STRUCT UNION are
    // set
    int access_specifier;

    string class_name_declaration;	// class
    // class_name_declaration

    int i_am_in_member;
    string member_declaration;	// member of class
    int member_definition_outside; // it must be a first declaration inside

    string declaration;

    int i_am_in_parameter_declaration;

    int just_reloaded;

    t_vector_decl_specifier param_vector_decl_specifier;

    c_class_member class_member;

    c_declarator declarator;

    int is_typedef;

    c_context() {
        i_token = 0;
        class_specifier_status = NO_CLASS_STATUS;
        class_key = 0;
        access_specifier = 0;

        class_name_declaration = "";

        i_am_in_member = 0;
        member_declaration = "";
        member_definition_outside = 0;

        declaration = "";

        i_am_in_parameter_declaration = 0;
        just_reloaded = 0;

        param_vector_decl_specifier.clear();
        class_member.clear();
        declarator.clear();

        is_typedef = 0;
    }
    void clear(void) {
        i_token = 0;
        class_specifier_status = NO_CLASS_STATUS;
        class_key = 0;
        access_specifier = 0;

        /*
         class A{
          class A_1{
          };
          class A_2{
          };
         };

         i need the class_name to chain A::A_1 A::A_2

            class_name_declaration = "";
        */

        i_am_in_member = 0;
        member_declaration = "";
        member_definition_outside = 0;

        declaration = "";

        i_am_in_parameter_declaration = 0;
        just_reloaded = 1;

        param_vector_decl_specifier.clear();

        class_member.clear();
        declarator.clear();

        is_typedef = 0;
    }
    void restore_but_not_i_token(c_context & context_param) {
        context_param.class_specifier_status = class_specifier_status;
        context_param.class_key = class_key;
        context_param.access_specifier = access_specifier;
        context_param.i_am_in_member = i_am_in_member;
        context_param.member_declaration = member_declaration;
        context_param.member_definition_outside = member_definition_outside;
        context_param.i_am_in_parameter_declaration =
            i_am_in_parameter_declaration;

        context_param.class_name_declaration = class_name_declaration;
        context_param.param_vector_decl_specifier =
            param_vector_decl_specifier;
        context_param.class_member = class_member;
        context_param.declarator = declarator;
        context_param.declaration = declaration;
        context_param.is_typedef = is_typedef;
    }
};

/*
 * this class save the context and restore it to support parsers'
 * backtraking
 */
struct c_context_tokens {
    // index of tokens
    // unsigned i_token;
    c_context context;

    c_context_tokens(c_context context_param);
    void save(c_context context_param);
    c_context restore(void);

    void restore_but_not_i_token(c_context & context_param);
};

class c_parser_descent
{
private:
    // int just_reloaded;
    // unsigned i_token_actual;
    // c_token token_actual;
    c_context context;
//  c_context qualified_id_context;

    t_tokens tokens_vector;

    // this is for cases like:
    //    typedef class{} t_A;
    int class_no_name_number;
    string class_no_name;
    string get_class_no_name(void);

    void tokens_vector_print(void);
    void tokens_vector_print_from_actual(void);

    void tokens_vector_reload(void);
    void tokens_vector_clear(void);

    void token_print(void);
    int token_get(void);
    int token_is(int id, c_trace_node trace_node);
    int token_is_not(int id , c_trace_node trace_node);
    c_token c_token_get(void);
    // void token_previous(void);
    int is_eof(c_trace_node trace_node);
    int preanalisys(int id, c_trace_node trace_node);
    int preanalisys_has_one(const int id[], c_trace_node trace_node);
    string colon_colon_chain;
    void colon_colon_chain_process(c_token & token);
    void token_next(string tab);

    // Expressions
    int preprocessor_include(c_trace_node trace_node);

    /*
     * rules from yacc
     */
    int error_recover(c_trace_node trace_node);

    // Translation unit.
    // this is the start rule in yacc
    int translation_unit(void);

    // Expressions
    int id_expression(c_trace_node trace_node);
    int unqualified_id(c_trace_node trace_node);
    int qualified_id(c_trace_node trace_node);
    int nested_name_specifier(c_trace_node trace_node);

    // Context-dependent identifiers.
    int typedef_name(c_trace_node trace_node);
    int class_name(c_trace_node trace_node);

    // Lexical elements.
    int identifier(c_trace_node trace_node);

    // Statements
    int statement(c_trace_node trace_node);
    int compound_statement(c_trace_node trace_node);
    int statement_seq(c_trace_node trace_node);

    // declarations
    int declaration_seq(c_trace_node trace_node);
    int declaration(c_trace_node trace_node);
    int block_declaration(c_trace_node trace_node);
    int simple_declaration(c_trace_node trace_node);
    int decl_specifier_seq(c_trace_node trace_node);
    int decl_specifier(c_trace_node trace_node);
    int storage_class_specifier(c_trace_node trace_node);
    int type_specifier(c_trace_node trace_node);
    int friend_specifier(c_trace_node trace_node);
    int typedef_specifier(c_trace_node trace_node);
    int function_specifier(c_trace_node trace_node);
    int simple_type_specifier(c_trace_node trace_node);
    int ptr_specifier(c_trace_node trace_node);    //rule mine
    int type_name(c_trace_node trace_node);
    int class_specifier(c_trace_node trace_node);
    int class_head(c_trace_node trace_node);
    int class_key(c_trace_node trace_node);
    int member_specification(c_trace_node trace_node);
    int member_declaration(c_trace_node trace_node);
    int member_declarator_list(c_trace_node trace_node);
    int member_declarator(c_trace_node trace_node);

    // Declarators
    int init_declarator_list(c_trace_node trace_node);
    int init_declarator(c_trace_node trace_node);
    int declarator(c_trace_node trace_node);
    int direct_declarator(c_trace_node trace_node);
    int ptr_operator(c_trace_node trace_node);
    int cv_qualifier_seq(c_trace_node trace_node);
    int cv_qualifier(c_trace_node trace_node);
    int declarator_id(c_trace_node trace_node);
    int parameter_declaration_clause(c_trace_node trace_node);
    int parameter_declaration_list(c_trace_node trace_node);
    int parameter_declaration(c_trace_node trace_node);
    int function_definition(c_trace_node trace_node);
    int function_body(c_trace_node trace_node);

    // Derived classes.
    int base_clause(c_trace_node trace_node);
    int base_specifier_list(c_trace_node trace_node);
    int base_specifier(c_trace_node trace_node);
    int access_specifier(c_trace_node trace_node);

    // Epsilon (optional) definitions.
    int declaration_seq_opt(c_trace_node trace_node);
    int COLONCOLON_opt(c_trace_node trace_node);
    int statement_seq_opt(c_trace_node trace_node);
    int decl_specifier_seq_opt(c_trace_node trace_node);
    int init_declarator_list_opt(c_trace_node trace_node);
    int identifier_opt(c_trace_node trace_node);
    int parameter_declaration_list_opt(c_trace_node trace_node);
    int ELLIPSIS_opt(c_trace_node trace_node);
    int member_specification_opt(c_trace_node trace_node);
    int base_clause_opt(c_trace_node trace_node);
    int member_declarator_list_opt(c_trace_node trace_node);

public:
    c_parser_descent();
    ~c_parser_descent();
    void yyparse(char *file_name);

    /*
     * tests
     */
    int test_01(void);
};

#endif

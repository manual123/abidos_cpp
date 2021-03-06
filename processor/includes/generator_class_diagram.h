/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_class_diagram.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 05/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef generator_class_diagram_h
#define generator_class_diagram_h

#include "symbols_table.h"

class c_generator_class_diagram
{
private:
    FILE * f_out;
    void members_url(t_vector_class_member & vector_class_member);
    void members_label(t_vector_class_member & vector_class_member);
    //classes structs typedefs ...
    void nodes(c_symbol & symbol);
    void inheritance(c_symbol & symbol);
    void friends(c_symbol & symbol);

    void members_compositions_aggregations(
        c_symbol & symbol,
        t_vector_class_member & vector_class_member
    );
    void compositions_aggregations(c_symbol & symbol);
    void typedef_members_compositions_aggregations(c_symbol & symbol);
    void typedef_label(c_symbol & symbol);
    void typedef_points_to(c_symbol & symbol);
public:
    void run(char *p_file_out);
};
#endif

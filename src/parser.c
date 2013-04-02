#include "scanner.h"


// Reports errors of parser
void error()
{
	error(0);
}

void error(int token)
{
    
}

// -------------------------- EBNF --------------------------------------------



void translation_unit(){
    
}

void storage_class_spec(){
    //TODO ADD STATIC IN TOKENS
}

void void_spec(){
    if(symbol == 1){
        getNextToken();
    }else if (symbol == 3){
        getNextToken();
    }else if (symbol == 2){
        getNextToken();
    }else if (symbol == 4){
        getNextToken();
    }else{
        error();
    }
}

void struct_or_union_spec(){
    
}

void struct_or_union(){
    if(symbol == 9){
        getNextToken();
    }else{
        error();
    }
}

void struct_decl_list(){
    
}

void init_declerator_list(){
    
}

void init_declarator(){
    
}

void struct_decl(){
    
}

void spec_qualifier_list(){
    
}

void struct_declarator_list(){
    
}

void struct_declarator(){
    
}

void enum_spec(){
    
}

void enumerator_list(){
    
}

void enumerator(){
    
}

void declarator(){
    
}

void direct_declarator(){
    
}

void pointer(){
    
}

void type_qualifier_list(){
    
}

void param_type_list(){
    
}

void param_list(){
    
}

void param_decl(){
    
}

void id_list(){
    
}

void initializer(){
    
}

void intializer_list(){
    
}

void type_name(){
    
}

void abstract_declarator(){
    
}

void direct_abstract_declarator(){
    
}

void typedef_name()
{
	id();
}

void stat()
{

}

void labeled_stat()
{
	
}

void exp_stat()
{

}

void compound_stat()
{

}

void stat_list()
{

}

void selection_stat()
{

}

void iteration_stat()
{

}

void jump_stat()
{

}

void exp()
{

}

void assignment_exp()
{

}

void assignment_operator()
{

}

void conditional_exp()
{

}

void const_exp()
{

}

void logical_or_exp()
{

}

void logical_and_exp()
{

}

void inclusive_or_exp()
{

}

void exclusive_or_exp()
{

}

void and_exp()
{

}

void equality_exp()
{

}

void relational_exp()
{

}

void shift_expression()
{

}

void additive_exp()
{

}

void mult_exp()
{

}

void cast_exp()
{

}

void unary_exp()
{

}

void unary_operator()
{

}

void postfix_exp()
{

}

void primary_exp()
{

}

void argument_exp_list()
{

}

void const()
{
	if(symbol == 200)
	{
		getNextToken();
	} else
	if(symbol == 202)
	{
		getNextToken();
	}
	else
	{
		error();
	}
}

// ----------------------------------------------------------------------------
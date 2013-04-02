#include "scanner.h"


// Reports errors of parser
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


// ----------------------------------------------------------------------------
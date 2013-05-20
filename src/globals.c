

static int FIRST_FUNCTION_DEFINITION;
static int FIRST_GLOBAL_VARIABLE_DECLARATION;
static int FIRST_TYPE;
static int FIRST_VARIABLE_DECLARATION;
static int FIRST_EXPRESSION;
static int FIRST_FUNCTION_STATEMENT;
static int FIRST_TYPE_DECLARATION;
static int FIRST_INSTRUCTION;
static int FIRST_TOP_DECLARATION;

//tokens:

//Keywords
static int TOKEN_NULL;
static int TOKEN_VOID;
static int TOKEN_INT;
static int TOKEN_CHAR;
static int TOKEN_DOUBLE;
static int TOKEN_WHILE;
static int TOKEN_IF;
static int TOKEN_ELSE;
static int TOKEN_RETURN;
static int TOKEN_STRUCT;
static int TOKEN_STATIC;
static int TOKEN_FOPEN;
static int TOKEN_FCLOSE;
static int TOKEN_FGETC;
static int TOKEN_FPUTC;
static int TOKEN_MALLOC;
static int TOKEN_SIZEOF;
static int TOKEN_TYPEDEF;

//Identififier
static int TOKEN_IDENTIFIER;

//Konstanten
static int TOKEN_CONSTINT;
static int TOKEN_CONSTDOUBLE;
static int TOKEN_CONSTCHAR;

//String-Literale
static int TOKEN_STRING_LITERAL;

//Operatoren
static int TOKEN_PLUS;
static int TOKEN_MINUS;
static int TOKEN_MULT;
static int TOKEN_DIVIDE;
static int TOKEN_PERCENT;
static int TOKEN_ASSIGNMENT;
static int TOKEN_EQUAL;
static int TOKEN_LESSEQUAL;
static int TOKEN_GREATEREQUAL;
static int TOKEN_LESS;
static int TOKEN_GREATER;
static int TOKEN_SHIFTLEFT;
static int TOKEN_SHIFTRIGHT;
static int TOKEN_ADDRESS;
static int TOKEN_AND;
static int TOKEN_BITWISEOR;
static int TOKEN_OR;
static int TOKEN_BITWISEEXCLOR;
static int TOKEN_BITWISENOT;
static int TOKEN_UNEQUAL;
static int TOKEN_INCREMENT;
static int TOKEN_DECREMENT;
static int TOKEN_NOT;
static int TOKEN_ACCESS;

//Punktuatoren

static int TOKEN_LSB;
static int TOKEN_RSB;
static int TOKEN_LRB;
static int TOKEN_RRB;
static int TOKEN_LCB;
static int TOKEN_RCB;
static int TOKEN_SEMICOLON;
static int TOKEN_COMMA;
static int TOKEN_COLON;
static int TOKEN_EOF;
static int TOKEN_INCLUDE;
static int TOKEN_POINT;

static int CLASS_VAR;
static int CLASS_TYPE;
static int CLASS_FIELD;

static int FORM_ARRAY;
static int FORM_RECORD;
static int FORM_INT;
static int FORM_CHAR;

static int errorCount;
static int warningCount;

// String literals:
static int isInString;
static int isInChar;

static int currentChar;
static int nextChar;

void initTokens(){
    TOKEN_NULL = 0;
    TOKEN_VOID = 1;
    TOKEN_INT = 2;
    TOKEN_CHAR = 3;
    TOKEN_DOUBLE = 4;
    TOKEN_WHILE = 5;
    TOKEN_IF = 6;
    TOKEN_ELSE = 7;
    TOKEN_RETURN = 8;
    TOKEN_STRUCT = 9;
    TOKEN_STATIC = 10;
    TOKEN_FOPEN = 11;
    TOKEN_FCLOSE = 12;
    TOKEN_FGETC = 13;
    TOKEN_FPUTC = 14;
    TOKEN_MALLOC = 15;
    TOKEN_SIZEOF = 16;
    TOKEN_TYPEDEF = 17;
    
    TOKEN_IDENTIFIER = 100;
    
    TOKEN_CONSTINT = 200;
    TOKEN_CONSTDOUBLE = 201;
    TOKEN_CONSTCHAR = 202;
    
    TOKEN_STRING_LITERAL = 300;
    
    TOKEN_PLUS = 400;
    TOKEN_MINUS = 401;
    TOKEN_MULT = 402;
    TOKEN_DIVIDE = 403;
    TOKEN_PERCENT = 404;
    TOKEN_ASSIGNMENT = 405;
    TOKEN_EQUAL = 406;
    TOKEN_LESSEQUAL = 407;
    TOKEN_GREATEREQUAL = 408;
    TOKEN_LESS = 409;
    TOKEN_GREATER = 410;
    TOKEN_SHIFTLEFT = 411;
    TOKEN_SHIFTRIGHT = 412;
    TOKEN_ADDRESS = 413;
    TOKEN_AND = 414;
    TOKEN_BITWISEOR = 415;
    TOKEN_OR = 416;
    TOKEN_BITWISEEXCLOR = 417;
    TOKEN_BITWISENOT = 418;
    TOKEN_UNEQUAL = 419;
    TOKEN_INCREMENT = 420;
    TOKEN_DECREMENT = 421;
    TOKEN_NOT = 422;
    TOKEN_ACCESS = 423;
    
    TOKEN_LSB = 500;
    TOKEN_RSB = 501;
    TOKEN_LRB = 502;
    TOKEN_RRB = 503;
    TOKEN_LCB = 504;
    TOKEN_RCB = 505;
    TOKEN_SEMICOLON = 506;
    TOKEN_COMMA = 507;
    TOKEN_COLON = 508;
    TOKEN_EOF = 509;
    TOKEN_INCLUDE = 510;
    TOKEN_POINT = 511;
    
    FIRST_EXPRESSION = 0;
    FIRST_FUNCTION_DEFINITION = 1;
    FIRST_GLOBAL_VARIABLE_DECLARATION = 2;
    FIRST_TYPE = 3;
    FIRST_VARIABLE_DECLARATION = 4;
    FIRST_FUNCTION_STATEMENT = 5;
    FIRST_TYPE_DECLARATION = 6;
    FIRST_INSTRUCTION = 7;
    FIRST_TOP_DECLARATION = 8;
    
    CLASS_FIELD = 0;
    CLASS_TYPE = 1;
    CLASS_VAR = 2;

    FORM_RECORD = 0;
    FORM_ARRAY = 1;
    FORM_INT = 2;
    FORM_CHAR = 3;
    
    errorCount = 0;
    warningCount = 0;
    
    isInString = 0;
    isInChar = 0;
    
    currentChar = -1;
    nextChar = -1;
}

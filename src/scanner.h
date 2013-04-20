int tokenType;
int intValue;
char stringValue[1024];
int lineNumber;
int column;

void getNextToken();


//tokens:

//Keywords
static int cbNull;
static int cbVoid;
static int cbInt;
static int cbChar;
static int cbDouble;
static int cbWhile;
static int cbIf;
static int cbElse;
static int cbReturn;
static int cbStruct;

//Identififier
static int Identifier;

//Konstanten
static int cbIntVar;
static int cbDoubleVar;
static int cbCharVar;

//String-Literale
static int cbSTringValue;

//Operatoren
static int cbPlus;
static int cbMinus;
static int cbMult;
static int cbDivide;
static int cbPercent;
static int cbAssignment;
static int cbEquals;
static int cbLessEqual;
static int cbGreaterEqual;
static int cbLess;
static int Greater;
static int cbShiftLeft;
static int cbShiftRight;
static int cbAdress;
static int cbAnd;
static int cbBitwiseOr;
static int cbOr;
static int cbBitewiseExclOr;
static int cbBitewiseNot;
static int cbUnequal;
static int cbIncrement;
static int cbDecrement;
static int cbNot;

//Punktuatoren

static int cbLSB;
static int cbRSB;
static int cbLRB;
static int cbRRB;
static int cbLCB;
static int cbRCB;
static int cbSemicolon;
static int cbComma;
static int cbColon;
static int cbEOF;
static int cbHash;
static int cbPoint;



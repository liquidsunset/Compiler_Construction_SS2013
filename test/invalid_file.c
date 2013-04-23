int main () {
	char c;
	c = 'X';
	printf("hihi");
	if(3<=5-2) {
		printf("hoho");
	}
}


void factor() {
    if(tokenType == ) {
        getNextToken();
        if(tokenType == TOKEN_LRB) // function call
        {
            getNextToken();
            while(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
                if(tokenType == TOKEN_COMMA)
                {
                    getNextToken();
                }
            }
            if(tokenType == TOKEN_RRB)
            {
                getNextToken();
            }
            else
            {
                mark(") missing");
                getNextToken();
            }
            return;
        }
        if(tokenType == TOKEN_LSB)
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
            }
            else
            {
                error("Expression for index expected");
            }
            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            else
            {
                mark("] missing");
                getNextToken();
                
                return;
            }
        }
        // if(tokenType == TOKEN_LSB) {
        //     getNextToken();
        //     expression();
        //     if(tokenType == TOKEN_RSB) {
        //         getNextToken();
        //     }
        //     else
        //     {
        //         mark(") missing");
        //         getNextToken();
        //     }
        //     return;
        // }
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            expression();
            if(tokenType == TOKEN_RRB)
            {
                getNextToken();
                return;
            }
            else
            {
                mark(") missing");
                getNextToken();
                return;
            }
        }
        if(tokenType == TOKEN_CONSTINT)
        {
            getNextToken();
            return;
            
            if(tokenType == TOKEN_CONSTCHAR)
            {
                getNextToken();
                return;
            }
            if(tokenType == TOKEN_STRING_LITERAL){
                getNextToken();
                return;
            }
            
            error("Factor expected");

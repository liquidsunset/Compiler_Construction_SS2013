// Reports errors of parser
void error()
{
	error(0);
}

void error(int token)
{

}

// -------------------------- EBNF --------------------------------------------

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
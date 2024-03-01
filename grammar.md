Program: Function+

Function: function type = ( (type identifier ',')* ) { Statement* }

Statement: type identifier;
	   type identifier = expression;
	   { Statement* }

Expression: integer_literal
	    identifier
	    identifier ( (Expression ',')* )
	    ( Expression )
		Expression '+' Expression
		Expression '*' Expression
	

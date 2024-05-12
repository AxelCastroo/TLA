%{

#include "BisonActions.h"

%}

%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char * varname;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <integer> INTEGER

%token <token> ADD
%token <token> SUB
%token <token> DIV
%token <token> MUL
%token <token> MOD

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET

%token <token> RED_BLACK_TREE
%token <token> BINARY_SEARCH_TREE
%token <token> EXPRESSION_TREE
%token <token> AVL_TREE

%token <token> NOT_EQUAL
%token <token> EQUAL
%token <token> LESS_EQUAL
%token <token> LESS
%token <token> GREATER_EQUAL
%token <token> GREATER

%token <token> AND
%token <token> OR
%token <token> COMMA
%token <token> ASSIGN
%token <token> NOT

%token <token> FOR
%token <token> IN
%token <token> TO
%token <token> IF
%token <token> ELSE

%token <token> IN_ORDER
%token <token> POST_ORDER
%token <token> PRE_ORDER

%token <token> INT_TYPE
%token <token> STRING_TYPE
%token <token> CONSTANT_TYPE
%token <token> BOOLEAN_TYPE

%token <token> INSERT
%token <token> REMOVE
%token <token> INCLUDES
%token <token> HEIGHT
%token <token> DEPTH
%token <token> CALCULATE
%token <token> VISUALIZE
%token <token> ITERATE

%token <token> DECLARATION

%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV
%left MOD
%left AND OR NOT
%left EQUAL NOT_EQUAL
%left LESS LESS_EQUAL GREATER GREATER_EQUAL
%left ASSIGN

%%

program: expression								{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression : expression[left] MOD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, MODULE_EXP); }
	| expression[left] AND expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, AND_EXP);}
	| expression[left] OR expression[right]							{ $$ = ArithmeticExpressionSemanticAction($left, $right, OR_EXP);}
	| NOT expression[exp]											{ $$ = ArithmeticExpressionSemanticAction(NULL , $exp, NOT_EXP);}
	| expression[left] EQUAL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, EQUAL_EXP);}
	| expression[left] NOT_EQUAL expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, NOT_EQUAL_EXP);}
	| expression[left] LESS expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, LESS_EXP);}
	| expression[left] LESS_EQUAL expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, LESS_EQUAL_EXP);}
	| expression[left] GREATER expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, GREATER_EXP);}
	| expression[left] GREATER_EQUAL expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, GREATER_EQUAL_EXP);}
	| expression[left] ADD expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION_EXP); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION_EXP); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION_EXP); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION_EXP); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%

/* program: statement_list
	;

statement_list: statement statement_list
	| statement
	;

statement: if_statement
	| for_statement
	| declaration_statement
	| assignment_statement
	| function_statement
	| iterator_statement
	;

block: OPEN_BRACKET statement_list CLOSE_BRACKET 
	;

if_statement: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block ELSE block
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block
	;

for_statement: FOR CONSTANT_TYPE IN OPEN_PARENTHESIS INTEGER TO INTEGER CLOSE_PARENTHESIS block
	;

declaration_statement: RED_BLACK_TREE DECLARATION 
	| BINARY_SEARCH_TREE DECLARATION
	| EXPRESSION_TREE DECLARATION tree_expression
	| AVL_TREE DECLARATION
	| INT_TYPE DECLARATION 
	| INT_TYPE DECLARATION ASSIGN tree_expression
	| INT_TYPE DECLARATION ASSIGN function_statement
	| BOOLEAN_TYPE DECLARATION
	| BOOLEAN_TYPE DECLARATION ASSIGN expression
	| BOOLEAN_TYPE DECLARATION ASSIGN function_statement			// TODO CONSTANT 
	;

assignment_statement: DECLARATION ASSIGN expression
	| DECLARATION ASSIGN tree_expression
	| DECLARATION ASSIGN function_statement
	;

function_statement: DECLARATION INSERT INTEGER;
	| DECLARATION REMOVE INTEGER
	| DECLARATION INCLUDES INTEGER
	| DECLARATION HEIGHT 
	| DECLARATION DEPTH INTEGER
	| DECLARATION CALCULATE
	| DECLARATION VISUALIZE
	| DECLARATION + DECLARATION
	| DECLARATION - DECLARATION
	;

iterator_statement: ITERATE OPEN_PARENTHESIS DECLARATION IN_ORDER CLOSE PARENTHESIS block
	| ITERATE OPEN_PARENTHESIS DECLARATION POST_ORDER CLOSE PARENTHESIS block
	| ITERATE OPEN_PARENTHESIS DECLARATION PRE_ORDER CLOSE PARENTHESIS block
	;

*/

/* 

BSTree bstree
bstree insert 4
Int number = 3
bstree insert number
Boolean result = bstree includes 3

for n in (1 to 10){

}

Int count = 0;
iterate(tree in-order){
 
}
*/

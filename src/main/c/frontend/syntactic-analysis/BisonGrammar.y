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
	
	StatementList statement_list;
	Statement * statement;
	ForStatement * for_statement;
	IfStatement * if_statement;
	RangeExpression * range_expression;
	Declaration * declaration;
	FunctionCall * function_call;
	Assignment * assignment;
	Block * block;
	IterateStatement iterator_statement;
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

%token <varname> DECLARATION

%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

%type <statement_list> statement_list
%type <statement> statement
%type <for_statement> for_statement
%type <if_statement> if_statement
%type <range_expression> range_expression
%type <declaration> declaration
%type <function_call> function_call
%type <assignment> assignment
%type <block> block
%type <iterator_statement> iterator_statement

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

program: statement_list												{ $$ = ProgramSemanticAction(currentCompilerState(), $1); }
	;

statement_list: statement statement_list							{ $$ = StatementListSemanticAction($1, $2); }
	| statement														{ $$ = StatementListSemanticAction($1, NULL); }									
	;

statement: if_statement												{ $$ = StatementSemanticAction($1, IF_STATEMENT); }
	| for_statement													{ $$ = StatementSemanticAction($1, FOR_STATEMENT); }
	| declaration													{ $$ = StatementSemanticAction($1, DECLARATION_STATEMENT); }
	| assignment													{ $$ = StatementSemanticAction($1, ASSIGNMENT_STATEMENT); }
	| function_call													{ $$ = StatementSemanticAction($1, FUNCTION_CALL_STATEMENT); }
	| iterator_statement 											{ $$ = StatementSemanticAction($1, ITERATE_STATEMENT); }
	;

block: OPEN_BRACKET statement_list[stat] CLOSE_BRACKET 				{ $$ = BlockSemanticAction($stat); }
	;

if_statement: IF OPEN_PARENTHESIS expression[exp] CLOSE_PARENTHESIS block[if] ELSE block[else]		{ $$ = IfStatementSemanticAction($exp, $if, $else); }
	| IF OPEN_PARENTHESIS expression[exp] CLOSE_PARENTHESIS block[bl]							{ $$ = IfStatementSemanticAction($exp, $bl, NULL); }
	;

for_statement: FOR DECLARATION[dec] IN range_expression[range] block[bl]							{ $$ = ForStatementSemanticAction($dec, $range, $bl); }
	;

range_expression: OPEN_PARENTHESIS expression[left] TO expression[right] CLOSE_PARENTHESIS			{ $$ = RangeExpressionSemanticAction($left, $right); }
	;

function_call: DECLARATION[dec] INSERT expression[exp]				{ $$ = FunctionCallSemanticAction($dec, $exp, INSERT_CALL); }
	| DECLARATION[dec] REMOVE expression[exp]						{ $$ = FunctionCallSemanticAction($dec, $exp, REMOVE_CALL); }
	| DECLARATION[dec] INCLUDES expression[exp]						{ $$ = FunctionCallSemanticAction($dec, $exp, INCLUDES_CALL); }
	| DECLARATION[dec] HEIGHT 										{ $$ = FunctionCallSemanticAction($dec, NULL, HEIGHT_CALL); }
	| DECLARATION[dec] DEPTH expression[exp]						{ $$ = FunctionCallSemanticAction($dec, $exp, DEPTH_CALL); }
	| DECLARATION[dec] CALCULATE									{ $$ = FunctionCallSemanticAction($dec, NULL, CALCULATE_CALL); }	
	| DECLARATION[dec] VISUALIZE									{ $$ = FunctionCallSemanticAction($dec, NULL,VISUALIZE_CALL); }
	| DECLARATION[dec] ADD DECLARATION								{ $$ = FunctionCallSemanticAction($dec, NULL, ADD_CALL); }
	| DECLARATION[dec] SUB DECLARATION								{ $$ = FunctionCallSemanticAction($dec, NULL,SUB_CALL); }
	;

iterator_statement: ITERATE OPEN_PARENTHESIS DECLARATION[dec] IN_ORDER[order] CLOSE_PARENTHESIS block[bl]			{ $$ = IterateSemanticAction($dec, $order, $bl); }
	| ITERATE OPEN_PARENTHESIS DECLARATION[dec] POST_ORDER[order] CLOSE_PARENTHESIS block[bl]						{ $$ = IterateSemanticAction($dec, $order, $bl); }
	| ITERATE OPEN_PARENTHESIS DECLARATION[dec] PRE_ORDER[order] CLOSE_PARENTHESIS block[bl]						{ $$ = IterateSemanticAction($dec, $order, $bl); }
	;

declaration: RED_BLACK_TREE DECLARATION[dec]						{ $$ = DeclarationSemanticAction($dec, RBT_DECLARATION); }
	| BINARY_SEARCH_TREE DECLARATION[dec]							{ $$ = DeclarationSemanticAction($dec, BST_DECLARATION); }
	| EXPRESSION_TREE DECLARATION[dec] expression					{ $$ = DeclarationSemanticAction($dec, EXP_DECLARATION); }
	| AVL_TREE DECLARATION[dec]										{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }
	| INT_TYPE DECLARATION[dec] 									{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }		// Fixed Soon
	| INT_TYPE DECLARATION[dec] ASSIGN expression					{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }
	| INT_TYPE DECLARATION[dec] ASSIGN function_call				{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }
	| BOOLEAN_TYPE DECLARATION[dec]									{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }
	| BOOLEAN_TYPE DECLARATION[dec] ASSIGN expression				{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }
	| BOOLEAN_TYPE DECLARATION[dec] ASSIGN function_call			{ $$ = DeclarationSemanticAction($dec, AVL_DECLARATION); }			
	| CONSTANT_TYPE DECLARATION[dec] ASSIGN expression				{ $$ = DeclarationSemanticAction($dec, CONST_DECLARATION); }
	;
	
assignment: DECLARATION[dec] ASSIGN expression[exp]					{ $$ = AssignmentSemanticAction($dec, $exp, NULL); }
	| DECLARATION[dec] ASSIGN function_call[fun]					{ $$ = AssignmentSemanticAction($dec, NULL, $fun); }
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

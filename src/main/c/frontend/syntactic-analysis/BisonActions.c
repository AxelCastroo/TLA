#include "BisonActions.h"
#include "../../backend/semantic-analysis/symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->intValue = value;
	constant->type = INT_CONSTANT;
	return constant;
}

Constant * BooleanConstantSemanticAction(const bool value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->boolValue = value;
	constant->type = BOOL_CONSTANT;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR_EXP;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->expression = NULL;
	factor->varName = NULL;
	factor->type = CONSTANT_FACTOR;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->constant = NULL;
	factor->varName = NULL;
	factor->type = EXPRESSION_FACTOR;
	return factor;
}

Factor * DeclarationFactorSemanticAction(char * varName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->varName = varName;
	factor->expression = NULL;
	factor->constant = NULL;
	factor->type = DECLARATION_FACTOR;
	return factor;
}

Program * ProgramSemanticAction(CompilerState * compilerState, StatementList statementList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->statements = statementList;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

StatementList StatementListSemanticAction(Statement *statement, StatementList next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList new = malloc(sizeof(struct StatementNode));
    new->statement = statement;
    new->next = next;
    return new;
}

Statement * StatementSemanticAction(void *statement, StatementType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Statement * new = calloc(1, sizeof(Statement));
	new->type = type;
    switch (type) {
        case IF_STATEMENT: new->ifStatement = statement; break;
        case FOR_STATEMENT: new->forStatement = statement; break;
        case ASSIGNMENT_STATEMENT: new->assignment = statement; break;
        case DECLARATION_STATEMENT: new->declaration = statement; break;
        case FUNCTION_CALL_STATEMENT: new->functionCall = statement; break;
		case ITERATE_STATEMENT: new->iterateStatement = statement; break;
    }

    return new;
}

IfStatement *IfStatementSemanticAction(Expression *cond, Block *if_block, Block *else_block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	if(getExpressionType(cond) != BOOL_VAR){
		logError(_logger, "Condition must be a boolean expression");
		exit(1);
	}

    IfStatementType type = IF_ELSE_TYPE;
    if (else_block == NULL) {
        type = IF_TYPE;
    }

    IfStatement * new = malloc(sizeof(IfStatement));
    new->type = type;
    new->condition = cond;
    new->blockIf = if_block;
    new->blockElse = else_block;
    return new;
}

ForStatement *ForStatementSemanticAction(char *varName, RangeExpression *range, Block *block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SymbolTableDeclareAux(varName, INT_DECLARATION, true);
	ForStatement * new = malloc(sizeof(ForStatement));
	new->varName = varName;
	new->range = range;
	new->block = block;
	return new;
}

RangeExpression *RangeExpressionSemanticAction(Expression *start, Expression *end) {
	_logSyntacticAnalyzerAction(__FUNCTION__);	

	if((start != NULL && getExpressionType(start) != INT_VAR) || (end && getExpressionType(end) != INT_VAR)){
		logError(_logger, "Invalid parameters type");
		exit(1);
	}

	RangeExpression * new = malloc(sizeof(RangeExpression));
	new->expressionLeft = start;
	new->expressionRight = end;
	return new;
}

Block *BlockSemanticAction(StatementList statementList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * new = malloc(sizeof(Block));
	new->statements = statementList;
	return new;
}

Assignment *AssignmentSemanticAction(char *varName, Expression *expression, FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	struct key key = {
		.varname = varName
	};

	struct value value;

	if(!symbolTableFind(&key, &value)){
		logError(_logger, "Variable %s undeclared", varName);
		exit(1);
	}

	if(value.type != INT_VAR && value.type != BOOL_VAR){
		logError(_logger, "Variable %s is not a defined data type", varName);
		exit(1);
	}

	if(functionCall != NULL && getFunctionCallType(functionCall) != value.type){
		logError(_logger, "Function return cannot be assigned to %s", varName);
		exit(1);
	}

	if(expression != NULL && getExpressionType(expression) != value.type){
		logError(_logger, "Expression cannot be assigned to %s", varName);
		exit(1);
	}

	value.metadata.hasValue = true;
	symbolTableInsert(&key, &value);

	Assignment * new = malloc(sizeof(Assignment));
	new->varName = varName;
	new->expression = expression;
	new->functionCall = functionCall;
	return new;
}

Declaration *DeclarationSemanticAction(char *varName, DeclarationType declarationType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VarType varType = SymbolTableDeclareAux(varName, declarationType, false);

	Declaration * new = malloc(sizeof(Declaration));
	new->varName = varName;
	new->assignment = NULL;
	new->type = varType;
	return new;
}

Declaration *BooleanDeclarationWithAssignmentSemanticAction(char *varName, Expression *expression, FunctionCall *functionCall){
	_logSyntacticAnalyzerAction(__FUNCTION__);

	VarType varType = SymbolTableDeclareAux(varName, BOOL_DECLARATION, true);

	if(functionCall != NULL && getFunctionCallType(functionCall) != BOOL_VAR){
		logError(_logger, "Function can not be assigned to %s", varName);
		exit(1);	
	}

	if(expression != NULL && getExpressionType(expression) != BOOL_VAR){
		logError(_logger, "Expression can not be assigned to %s", varName);
		exit(1);
	}

	Assignment * assignment = malloc(sizeof(Assignment));
	assignment->varName = varName;
	assignment->expression = expression;
	assignment->functionCall = functionCall;

	Declaration * new = malloc(sizeof(Declaration));
	new->varName = varName;
	new->assignment = assignment;
	return new;
}

Declaration *IntegerDeclarationWithAssignmentSemanticAction(char *varName, Expression *expression, FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	VarType varType = SymbolTableDeclareAux(varName, INT_DECLARATION, true);

	if(functionCall != NULL && getFunctionCallType(functionCall) != INT_VAR){
		logError(_logger, "Function can not be assigned to %s", varName);
		exit(1);
	}

	if(expression != NULL && getExpressionType(expression) != INT_VAR){
		logError(_logger, "Expression can not be assigned to %s", varName);
		exit(1);
	}

	Assignment * assignment = malloc(sizeof(Assignment));
	assignment->varName = varName;
	assignment->expression = expression;
	assignment->functionCall = functionCall;

	Declaration * new = malloc(sizeof(Declaration));
	new->varName = varName;
	new->assignment = assignment;
	return new;
}

FunctionCall *FunctionCallSemanticAction(char *varName, Expression *expression, FunctionCallType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	struct key key = {
		.varname = varName
	};

	if(!symbolTableFind(&key, NULL)){
		logError(_logger, "Variable %s undeclared", varName);
		exit(1);
	}

	FunctionCall * new = malloc(sizeof(FunctionCall));
    new->type = type;
    new->varName = varName;
    new->expression = expression;
    new->declaration = NULL;
    return new;
}

IterateStatement *IterateSemanticAction(char *varName, IteratorType type, Block *block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	struct key key = {
		.varname = varName
	};

	if(!symbolTableFind(&key, NULL)){
		logError(_logger, "Variable %s undeclared", varName);
		exit(1);
	}

	if(type != INORDER && type != POSTORDER && type != PREORDER){
		logError(_logger, "Invalid order type");
		exit(1);
	}

	IterateStatement * new = malloc(sizeof(IterateStatement));
	new->varName = varName;
	new->type = type;
	new->block = block;
	return new;
}

static VarType SymbolTableDeclareAux(char *varname, DeclarationType type, bool hasValue){
	VarType varType;
	struct metadata metadata = {
		.hasValue = hasValue
	};

	switch (type){
		case INT_DECLARATION:
			varType = INT_VAR; break;
		case BOOL_DECLARATION:
			varType = BOOL_VAR; break;
		case RBT_DECLARATION: 
			varType = RBT_VAR; break;
		case AVL_DECLARATION:
			varType = AVL_VAR; break;
		case BST_DECLARATION:
			varType = BST_VAR; break;
		case EXP_DECLARATION: 
			varType = EXP_VAR; break;
		case CONST_DECLARATION:
			varType = CONST_VAR; break;
	}

	struct key key = {
		.varname = varname
	};

	if(symbolTableFind(&key, NULL)){
		logError(_logger, "Invalid redeclaration of varaible %s", varname);
		exit(1);
	}

	struct value value = {
		.type = varType, 
		.metadata = metadata
	};
	
	symbolTableInsert(&key, &value);
	return varType;
}

static int getFunctionCallType(FunctionCall *functionCall){
	switch(functionCall->type){
		case HEIGHT_CALL:
		case DEPTH_CALL:
			return INT_VAR;
		case INCLUDES_CALL:
			return BOOL_VAR;
		default:
			return -1;
	}
}

static int getExpressionType(Expression *expression){
	struct key key;
	struct value value;
	switch (expression->type){
		case ADDITION_EXP:
		case SUBTRACTION_EXP:
		case MULTIPLICATION_EXP:
		case DIVISION_EXP:
		case MODULE_EXP:
			return (getExpressionType(expression->leftExpression) == INT_VAR && getExpressionType(expression->rightExpression) == INT_VAR) ? INT_VAR : -1;
		case AND_EXP:
		case OR_EXP:	
			return (getExpressionType(expression->leftExpression) == BOOL_VAR && getExpressionType(expression->rightExpression) == BOOL_VAR) ? BOOL_VAR : -1;
		case NOT_EXP:
			return (getExpressionType(expression->leftExpression) == BOOL_VAR) ? BOOL_VAR : -1;
		case EQUAL_EXP:
		case NOT_EQUAL_EXP:
		case LESS_EQUAL_EXP:
		case LESS_EXP:
		case GREATER_EQUAL_EXP:
		case GREATER_EXP:
			return (getExpressionType(expression->leftExpression) == getExpressionType(expression->rightExpression)) ? BOOL_VAR : -1;
		case FACTOR_EXP:
			switch(expression->factor->type){
				case CONSTANT_FACTOR:
					return expression->factor->constant->type == INT_CONSTANT ? INT_VAR : BOOL_VAR;
				case EXPRESSION_FACTOR: 
					return getExpressionType(expression->factor->expression);
				case DECLARATION_FACTOR:
					key.varname = expression->factor->varName;
					if(!symbolTableFind(&key, &value)){
						return INT_VAR;
					}
					return value.type;
				default:
					logError(_logger, "Invalid factor type");
					exit(1);
			}
		default: 
			logError(_logger, "Invalid expression type");
			exit(1);
	}
}
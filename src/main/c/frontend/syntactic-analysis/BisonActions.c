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

static struct key *usedSymbols;
static VarType *usedSymbolsExpectedType;
static int usedSymbolsCount = 0;

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = malloc(sizeof(Constant));
	constant->intValue = value;
	constant->type = INT_CONSTANT;
	return constant;
}

Constant * BooleanConstantSemanticAction(const bool value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = malloc(sizeof(Constant));
	constant->boolValue = value;
	constant->type = BOOL_CONSTANT;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	VarType leftType, rightType;

	if(leftExpression != NULL && rightExpression != NULL && (leftType = getExpressionType(leftExpression)) != (rightType = getExpressionType(rightExpression))){
		logError(_logger, "Incompatible parameters");
		exit(1);
	}

	VarType factorType;

	if(type == AND_EXP || type == OR_EXP || type == NOT_EXP){
		factorType = BOOL_VAR;
	}
	else {
		factorType = INT_VAR;
	}

	if(leftExpression != NULL && leftExpression->type == FACTOR_EXP && leftExpression->factor->type == DECLARATION_FACTOR){
		AddUsedSymbol(leftExpression->factor->varName, factorType);
	}

	if(rightExpression != NULL && rightExpression->type == FACTOR_EXP && rightExpression->factor->type == DECLARATION_FACTOR){
		AddUsedSymbol(rightExpression->factor->varName, factorType);
	}

	Expression * expression = malloc(sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	expression->factor = NULL;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = malloc(sizeof(Expression));
	expression->factor = factor;
	expression->leftExpression = NULL;
	expression->rightExpression = NULL;
	expression->type = FACTOR_EXP;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = malloc(sizeof(Factor));
	factor->constant = constant;
	factor->expression = NULL;
	factor->varName = NULL;
	factor->type = CONSTANT_FACTOR;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = malloc(sizeof(Factor));
	factor->expression = expression;
	factor->constant = NULL;
	factor->varName = NULL;
	factor->type = EXPRESSION_FACTOR;
	return factor;
}

Factor * DeclarationFactorSemanticAction(char * varName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = malloc(sizeof(Factor));
	factor->varName = varName;
	factor->expression = NULL;
	factor->constant = NULL;
	factor->type = DECLARATION_FACTOR;
	return factor;
}

Program * ProgramSemanticAction(CompilerState * compilerState, StatementList statementList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	validateUsedSymbols();
	free(usedSymbols);
	free(usedSymbolsExpectedType);
	Program * program = malloc(sizeof(Program));
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

	if(value.type != INT_VAR && value.type != BOOL_VAR && value.type != EXP_VAR){
		logError(_logger, "Variable %s is not an integer or boolean data type", varName);
		exit(1);
	}

	if(functionCall != NULL && getFunctionCallType(functionCall) != value.type){
		logError(_logger, "Function return cannot be assigned to %s", varName);
		exit(1);
	}

	//muere aca
	if(expression != NULL && getExpressionType(expression) != value.type && value.type != EXP_VAR){
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
	new->type = varType;
	return new;
}

FunctionCall *FunctionCallSemanticAction(char *varName, Expression *expression, FunctionCallType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	struct key key = {
		.varname = varName
	};

	struct value value;

	if(!symbolTableFind(&key, &value)){
		logError(_logger, "Variable %s undeclared", varName);
		exit(1);
	}

	if (value.type != RBT_VAR && value.type != AVL_VAR && value.type != BST_VAR && value.type != EXP_VAR) {
        logError(_logger, "Variable %s is not a tree type", varName);
        exit(1);
    } else {
		if (value.type == EXP_VAR && type != CALCULATE_CALL){
			logError(_logger, "Variable %s is not a valid tree type", varName);
        	exit(1);
		}
		if (value.type != EXP_VAR && type == CALCULATE_CALL){
			logError(_logger, "Variable %s is not an expression tree type", varName);
        	exit(1);
		}
	}

	FunctionCall * new = malloc(sizeof(FunctionCall));
    new->type = type;
    new->varName = varName;
    new->expression = expression;
    new->declaration = NULL;
    return new;
}

IterateStatement *IterateSemanticAction(char *varName, IteratorType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	struct key key = {
		.varname = varName
	};

	struct value value;

	if(!symbolTableFind(&key, &value)){
		logError(_logger, "Variable %s undeclared", varName);
		exit(1);
	}

	if (value.type != RBT_VAR && value.type != AVL_VAR && value.type != BST_VAR) {
        logError(_logger, "Variable %s is not a tree type", varName);
        exit(1);
    }

	if(type != INORDER && type != POSTORDER && type != PREORDER){
		logError(_logger, "Invalid order type");
		exit(1);
	}


	IterateStatement * new = malloc(sizeof(IterateStatement));
	new->varName = varName;
	new->type = type;
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
		logError(_logger, "Invalid redeclaration of variable");
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
			return getExpressionType(expression->leftExpression) == BOOL_VAR ? BOOL_VAR : -1;
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

static void AddUsedSymbol(char *varname, VarType expectedType){
	struct key key = {.varname = varname};

    for (int i = 0; i < usedSymbolsCount; i++) {
        if (strcmp(usedSymbols[i].varname, varname) == 0) {
            if (usedSymbolsExpectedType[i] != expectedType) {
                logError(_logger, "Variable %s conflicting types", varname);
                exit(1);
            }
            return;
        }
    }

    if (usedSymbolsCount == 0) {
        usedSymbols = malloc(sizeof(struct key));
        usedSymbolsExpectedType = malloc(sizeof(VarType));
    } else if (usedSymbolsCount % 10 == 0) {
        usedSymbols = realloc(usedSymbols, sizeof(struct key) * (usedSymbolsCount + 10));
        usedSymbolsExpectedType = realloc(usedSymbolsExpectedType, sizeof(VarType) * (usedSymbolsCount + 10));
    }

    printf("Adding %s\n", varname);

    usedSymbols[usedSymbolsCount] = key;
    usedSymbolsExpectedType[usedSymbolsCount++] = expectedType;

    printf("Added %s\n", usedSymbols[usedSymbolsCount - 1].varname);
}

static void validateUsedSymbols() {
    for (int i = 0; i < usedSymbolsCount; i++) {
        struct key key = usedSymbols[i];
        struct value value;
        if (!symbolTableFind(&key, &value)) {
            logError(_logger, "Variable %s undeclared", key.varname);
            exit(1);
        }

        if (value.metadata.hasValue == false) {
            logError(_logger, "Variable %s has no value", key.varname);
            exit(1);
        }

        if (value.type != usedSymbolsExpectedType[i]) {
            logError(_logger, "Variable %s is not of type %d", key.varname, usedSymbolsExpectedType[i]);
            exit(1);
        }
    }
}
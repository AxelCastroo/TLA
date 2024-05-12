#include "BisonActions.h"

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
	return constant;
}

Constant * BooleanConstantSemanticAction(const bool value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->boolValue = value;
	return constant;
}

// Constant * ConstantSemanticAction(const int value, const bool value) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Constant * constant = calloc(1, sizeof(Constant));
// 	constant->intValue = value;
// 	constant->boolValue = value;
// 	return constant;
// }

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
	factor->type = CONSTANT_FACTOR;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION_FACTOR;
	return factor;
}

Factor * DeclarationFactorSemanticAction(char * varName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->varName = varName;
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
        default: assert(0 && "Illegal State"); break;
    }

    return new;
}

IfStatement *IfStatementSemanticAction(Expression *cond, Block *if_block, Block *else_block) {
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
	ForStatement * new = malloc(sizeof(ForStatement));
	new->varName = varName;
	new->range = range;
	new->block = block;
	return new;
}

RangeExpression *RangeExpressionSemanticAction(Expression *start, Expression *end) {
	RangeExpression * new = malloc(sizeof(RangeExpression));
	new->expressionRight = start;
	new->expressionLeft = end;
	return new;
}

Block *BlockSemanticAction(StatementList statementList) {
	Block * new = malloc(sizeof(Block));
	new->statements = statementList;
	return new;
}

Assignment *AssignmentSemanticAction(char *varName, Expression *expression, FunctionCall *functionCall) {
	Assignment * new = malloc(sizeof(Assignment));
	new->varName = varName;
	new->expression = expression;
	new->functionCall = functionCall;
	return new;
}

Declaration *DeclarationSemanticAction(char *varName, DeclarationType declarationType) {
	Declaration * new = malloc(sizeof(Declaration));
	new->varName = varName;
	new->assignment = NULL;
	return new;
}

Declaration *DeclarationWithAssignmentSemanticAction(char *varName, DeclarationType declarationType, Expression *expression, FunctionCall *functionCall) {
	Declaration * new = malloc(sizeof(Declaration));
	new->varName = varName;
	new->assignment = NULL;
	return new;
}

FunctionCall *FunctionCallSemanticAction(char *varName, Expression *expression, FunctionCallType type) {
	FunctionCall * new = malloc(sizeof(FunctionCall));
    new->type = type;
    new->varName = varName;
    new->expression = expression;
    new->declaration = NULL;
    return new;
}

IterateStatement *IterateSemanticAction(char *varName, IteratorType type, Block *block) {
	IterateStatement * new = malloc(sizeof(IterateStatement));
	new->varName = varName;
	new->type = type;
	new->block = block;
	return new;
}



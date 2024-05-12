#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

/*
void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION_EXP:
			case DIVISION_EXP:
			case MULTIPLICATION_EXP:
			case SUBTRACTION_EXP:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR_EXP:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT_FACTOR:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION_FACTOR:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
}
*/

void releaseProgram(Program *program) {
    if (program == NULL) {
        return;
    }

    releaseStatementList(program->statements);
    free(program);
}

void releaseStatementList(StatementList statementList) {
    if (statementList == NULL) {
        return;
    }

    releaseStatementList(statementList->next);

    releaseStatement(statementList->statement);
    free(statementList);
}

void releaseStatement(Statement *statement) {
    if (statement == NULL) {
        return;
    }

    switch (statement->type) {
        case IF_STATEMENT:
            releaseIfStatement(statement->ifStatement);
            break;
        case FOR_STATEMENT:
            releaseForStatement(statement->forStatement);
            break;
        case FUNCTION_CALL_STATEMENT:
            releaseFunctionCall(statement->functionCall);
            break;
        case DECLARATION_STATEMENT:
            releaseDeclaration(statement->declaration);
            break;
        case ASSIGNMENT_STATEMENT:
            releaseAssignment(statement->assignment);
            break;
        default:
            break;
    }
    free(statement);
}

void releaseAssignment(Assignment *assignment) {
    if (assignment == NULL) {
        return;
    }

    releaseExpression(assignment->expression);
    releaseVariable(assignment->varName);
    releaseFunctionCall(assignment->functionCall);
    free(assignment);
}

void releaseFunctionCall(FunctionCall *functionCall) {
    if (functionCall == NULL) {
        return;
    }

    releaseExpression(functionCall->expression);
    releaseVariable(functionCall->varName);
    releaseDeclaration(functionCall->declaration);
    free(functionCall);
}

void releaseDeclaration(Declaration *declaration) {
    if (declaration == NULL) {
        return;
    }

    releaseVariable(declaration->varName);
    free(declaration);
}


void releaseForStatement(ForStatement *forStatement) {
    if (forStatement == NULL) {
        return;
    }

    releaseRangeExpression(forStatement->range);
    releaseBlock(forStatement->block);
    free(forStatement);
}

void releaseRangeExpression(RangeExpression *rangeExpression) {
    if (rangeExpression == NULL) {
        return;
    }

    releaseExpression(rangeExpression->expressionLeft);
    releaseExpression(rangeExpression->expressionRight);
    free(rangeExpression);
}

void releaseIfStatement(IfStatement *ifStatement) {
    if (ifStatement == NULL) {
        return;
    }

    releaseExpression(ifStatement->condition);
    releaseBlock(ifStatement->blockIf);

    if (ifStatement->type == IF_ELSE_TYPE) {
        releaseBlock(ifStatement->blockElse);
    }
    free(ifStatement);
}

void releaseBlock(Block *block) {
    if (block == NULL) {
        return;
    }

    releaseStatementList(block->statements);
    free(block);
}

void releaseExpression(Expression *expression) {
    if (expression == NULL) {
        return;
    }

    if (expression->type == EXPRESSION_FACTOR) {
        releaseFactor(expression->factor);
    } else {
        releaseExpression(expression->leftExpression);
        releaseExpression(expression->rightExpression);
    }

    free(expression);
}

void releaseFactor(Factor *factor) {
    if (factor == NULL) {
        return;
    }

    switch (factor->type) {
        case EXPRESSION_FACTOR:
            releaseExpression(factor->expression);
            break;
        case DECLARATION_FACTOR:
            releaseVariable(factor->varName);
            break;
        case CONSTANT_FACTOR:
            releaseConstant(factor->constant);
            break;
    }

    free(factor);
}

void releaseConstant(Constant *constant) {
    if (constant == NULL) {
        return;
    }

    free(constant);
}

void releaseVariable(char *varName) {
    if (varName == NULL) {
        return;
    }
    free(varName);
}

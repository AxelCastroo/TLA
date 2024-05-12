#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>
#include <assert.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Program * ProgramSemanticAction(CompilerState * compilerState, StatementList statementList);
StatementList StatementListSemanticAction(Statement *statement, StatementList next);
Statement * StatementSemanticAction(void *statement, StatementType type);
IfStatement *IfStatementSemanticAction(Expression *cond, Block *if_block, Block *else_block);
ForStatement *ForStatementSemanticAction(char *varName, RangeExpression *range, Block *block);
RangeExpression *RangeExpressionSemanticAction(Expression *start, Expression *end);
Block *BlockSemanticAction(StatementList statementList);
Assignment *AssignmentSemanticAction(char *varName, Expression *expression, FunctionCall *functionCall);
Declaration *DeclarationSemanticAction(char *varName, DeclarationType declarationType);
FunctionCall *FunctionCallSemanticAction(char *varName, Expression *expression, FunctionCallType type);
IterateStatement *IterateSemanticAction(char *varName, IteratorType type, Block *block);
#endif

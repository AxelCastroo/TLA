#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../../backend/semantic-analysis/symbolTable.h"

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;

typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct StatementNode * StatementList;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

typedef enum {
    INT_CONSTANT,
    BOOL_CONSTANT,
} ConstantType;

typedef struct {
	union {
		int intValue;
    	bool boolValue;
	};
	ConstantType type;
} Constant;

typedef enum {
	EXPRESSION_FACTOR,
	CONSTANT_FACTOR,
	DECLARATION_FACTOR,
} FactorType;

typedef struct {
	VarType type;
	char * varName;
    struct Assignment * assignment;
} Declaration;

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
		Declaration * declaration;
	};
	FactorType type;
	char * varName;
};

enum ExpressionType {
	ADDITION_EXP,
	DIVISION_EXP,
	FACTOR_EXP,
	MULTIPLICATION_EXP,
	SUBTRACTION_EXP,
	MODULE_EXP,
	EQUAL_EXP,
	NOT_EQUAL_EXP,
	GREATER_EXP,
	GREATER_EQUAL_EXP,
	LESS_EXP,
	LESS_EQUAL_EXP,
	OR_EXP,
	AND_EXP,
	NOT_EXP
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
};

typedef struct {
	StatementList statements;
} Block;

typedef enum {
	IF_ELSE_TYPE,
	IF_TYPE,
} IfStatementType;

typedef struct {
	IfStatementType type;
	Expression * condition;
	Block * blockIf;
	Block * blockElse;
} IfStatement;

typedef struct {
	Expression * expressionLeft;
	Expression * expressionRight;
} RangeExpression;

typedef struct {
	char * varName;
	RangeExpression * range;
	Block * block;
} ForStatement;

typedef enum {
    RBT_DECLARATION,
    BST_DECLARATION,
    AVL_DECLARATION,
	EXP_DECLARATION,
	INT_DECLARATION,
    BOOL_DECLARATION,
	CONST_DECLARATION,
} DeclarationType;

typedef enum{
	INORDER,
	PREORDER,
	POSTORDER
}IteratorType;

typedef struct {
	char * varName;
	IteratorType type;
	Block * block;
} IterateStatement;

typedef enum {
	INSERT_CALL,
	REMOVE_CALL,
	INCLUDES_CALL,
	HEIGHT_CALL,
	DEPTH_CALL,
	CALCULATE_CALL,
	VISUALIZE_CALL,
	ADD_CALL,
	SUB_CALL
} FunctionCallType;

typedef struct {
	char * varName;
	Expression * expression;
	Declaration * declaration;
	FunctionCallType type;
} FunctionCall;

typedef struct Assignment {
	char * varName;
	Expression * expression;
    FunctionCall * functionCall;
} Assignment;

typedef enum {
	IF_STATEMENT,
	FOR_STATEMENT,
	FUNCTION_CALL_STATEMENT,
	DECLARATION_STATEMENT,
	ASSIGNMENT_STATEMENT,
	ITERATE_STATEMENT
} StatementType;

typedef struct {
	union {
		IfStatement * ifStatement;
		ForStatement * forStatement;
		FunctionCall * functionCall;
		Declaration * declaration;
		Assignment * assignment;
		IterateStatement * iterateStatement;
	};
	StatementType type;
} Statement;

struct StatementNode {
	Statement * statement;
	StatementList next;
};

struct Program {
	StatementList statements;
};

/**
 * Node recursive destructors.
 */
void releaseProgram(Program * program);
void releaseStatementList(StatementList statementList);
void releaseStatement(Statement * statement);
void releaseAssignment(Assignment * assignment);
void releaseFunctionCall(FunctionCall * functionCall);
void releaseDeclaration(Declaration * declaration);
void releaseForStatement(ForStatement * forStatement);
void releaseRangeExpression(RangeExpression * rangeExpression);
void releaseIfStatement(IfStatement * ifStatement);
void releaseBlock(Block * block);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseConstant(Constant *constant);
void releaseVariable(char * variable);

#endif

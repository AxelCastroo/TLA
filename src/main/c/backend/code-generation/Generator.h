#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdio.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
void generate(CompilerState * compilerState);

/** PRIVATE FUNCTIONS */
static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(void);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static void _generateStatementList(StatementList statementList);
static void _generateStatement(const unsigned int indentationLevel, Statement * statement);
static void _generateBlock(const unsigned int indentationLevel, Block * block);
static void _generateIfStatement(const unsigned int indentationLevel, IfStatement * ifStatement);
static void _generateForStatement(const unsigned int indentationLevel, ForStatement * forStatement);
static void _generateFunctionCall(const unsigned int indentationLevel, FunctionCall * functionCall);
static void _generateDeclaration(const unsigned int indentationLevel, Declaration * declaration);
static void _generateAssignment(const unsigned int indentationLevel, Assignment * assignment);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

#endif

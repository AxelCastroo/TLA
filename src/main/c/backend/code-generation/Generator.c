#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdarg.h>

static FILE *outputFile;

void SetOutputFile(FILE *file) {
    outputFile = file;
}


/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
    _logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

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
static void _generateIterate(const unsigned int indentationLevel, IterateStatement * iterateStatement);
static void _generateFunctionCall(const unsigned int indentationLevel, FunctionCall * functionCall);
static void _generateDeclaration(const unsigned int indentationLevel, Declaration * declaration);
static void _generateAssignment(const unsigned int indentationLevel, Assignment * assignment);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts an expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static const char _expressionTypeToCharacter(const ExpressionType type) {
    switch (type) {
        case ADDITION_EXP: return '+';
        case DIVISION_EXP: return '/';
        case MULTIPLICATION_EXP: return '*';
        case SUBTRACTION_EXP: return '-';
        case MODULE_EXP: return '%';
        default:
            logError(_logger, "The specified expression type cannot be converted into character: %d", type);
            return '\0';
    }
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
    if (constant->type == INT_CONSTANT){
        _output(indentationLevel, "%d", constant->intValue);
    }
    else {
        _output(indentationLevel, "%s", constant->boolValue ? "true" : "false");
    }
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Java program.
 */
static void _generateEpilogue(void) {
    _output(1, "}\n");
    _output(0, "}\n");
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
    switch (expression->type) {
        case ADDITION_EXP:
        case DIVISION_EXP:
        case MULTIPLICATION_EXP:
        case SUBTRACTION_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " %c ", _expressionTypeToCharacter(expression->type));
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case AND_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " && ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case NOT_EXP: 
            _output(indentationLevel, "!");
            _generateExpression(indentationLevel, expression->leftExpression);
            break;
        case OR_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " || ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case EQUAL_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " == ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case NOT_EQUAL_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " != ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case LESS_EXP: 
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " < ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case LESS_EQUAL_EXP: 
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " <= ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case GREATER_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " > ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case GREATER_EQUAL_EXP:
            _generateExpression(indentationLevel, expression->leftExpression);
            _output(indentationLevel, " >= ");
            _generateExpression(indentationLevel, expression->rightExpression);
            break;
        case FACTOR_EXP:
            _generateFactor(indentationLevel, expression->factor);
            break;
        default:
            logError(_logger, "The specified expression type is unknown: %d", expression->type);
            break;
    }
}

/**
 * Generates the output of a factor.
 */
static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
    switch (factor->type) {
        case CONSTANT_FACTOR:
            _generateConstant(indentationLevel, factor->constant);
            break;
        case EXPRESSION_FACTOR:
            _output(indentationLevel, "(");
            _generateExpression(indentationLevel, factor->expression);
            _output(indentationLevel, ")");
            break;
        case DECLARATION_FACTOR: 
            _output(indentationLevel, "%s", factor->varName);
            break;
        default:
            logError(_logger, "The specified factor type is unknown: %d", factor->type);
            break;
    }
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    _output(0, "import java.io.IOException;\n");
    _output(0, "public class Main {\n");
    _output(1, "public static void main(String[] args) throws IOException{\n");
    _generateStatementList(program->statements);
}

/** 
 * Generates the output of a statementlist.  
 */
static void _generateStatementList(StatementList statementList){
    StatementList current = statementList;
    while (current != NULL) {
        _generateStatement(2, current->statement);
        current = current->next;
    }
}

/** 
 * Generates the output of a declaration. 
 */
static void _generateDeclaration(const unsigned int indentationLevel, Declaration * declaration){
    switch (declaration->type){
    case RBT_DECLARATION:
        _output(indentationLevel, "Tree<Integer> %s = new RBT<> ();\n", declaration->varName);
        break;
    case AVL_DECLARATION:
        _output(indentationLevel, "Tree<Integer> %s = new AVL<>(); \n", declaration->varName);
        break;
    case BST_DECLARATION:
        _output(indentationLevel, "Tree<Integer> %s = new BST<>();\n", declaration->varName);
        break;
    case EXP_DECLARATION:
        _output(indentationLevel, "EXP<String> %s = new EXP<>(); \n", declaration->varName);
        break;
    case INT_DECLARATION:
        _output(indentationLevel, "int ");
        if (declaration->assignment != NULL){
            _generateAssignment(0, declaration->assignment);
            _output(0, ";\n");
        }
        else {
            _output(0, "%s;\n", declaration->varName);
        }
        break;
    case BOOL_DECLARATION:
        _output(indentationLevel, "boolean ");
        if(declaration->assignment != NULL){
            _generateAssignment(indentationLevel, declaration->assignment);
            _output(0, ";\n");
        }
        else {
            _output(0, "%s;\n", declaration->varName);
        }
        break;
    default:
        break;
    }
}

/** 
 * Generates the output of an assignment. 
 */
static void _generateAssignment(const unsigned int indentationLevel, Assignment * assignment){
    _output(indentationLevel, "%s = ", assignment->varName);
    if(assignment->expression != NULL){
        _generateExpression(0, assignment->expression);
    }
    if(assignment->functionCall != NULL){
        _generateFunctionCall(0, assignment->functionCall);
    }
}

/**
 * Generates the prologue of the generated output, starting a valid Java program.
 */
static void _generatePrologue(void) {
    _output(0, "import java.util.*;\n");
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
    return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
    va_list arguments;
    va_start(arguments, format);
    char * indentation = _indentation(indentationLevel);
    char * effectiveFormat = concatenate(2, indentation, format);
    vfprintf(outputFile, effectiveFormat, arguments);
    free(effectiveFormat);
    free(indentation);
    va_end(arguments);
}

/**
 * Generates a statement.
 */
static void _generateStatement(const unsigned int indentationLevel, Statement * statement) {
    switch (statement->type) {
        case IF_STATEMENT:
            _generateIfStatement(indentationLevel, statement->ifStatement);
            break;
        case FOR_STATEMENT:
            _generateForStatement(indentationLevel, statement->forStatement);
            break;
        case FUNCTION_CALL_STATEMENT:
            _generateFunctionCall(indentationLevel, statement->functionCall);
            break;
        case DECLARATION_STATEMENT:
            _generateDeclaration(indentationLevel, statement->declaration);
            break;
        case ASSIGNMENT_STATEMENT:
            _generateAssignment(indentationLevel, statement->assignment);
            _output(0, ";\n");
            break;
        case ITERATE_STATEMENT:
            _generateIterate(indentationLevel, statement->iterateStatement);
            break;
        default:
            logError(_logger, "Unknown statement type: %d", statement->type);
            break;
    }
}

static void _generateIterate(const unsigned int indentationLevel, IterateStatement * iterateStatement){
    _output(indentationLevel, "%s", iterateStatement->varName);
    switch (iterateStatement->type){
    case INORDER:
        _output(0, ".inorder()");
        break;
    case POSTORDER:
        _output(0, ".postorder()");
        break;
    case PREORDER:
        _output(0, ".preorder()");
        break;
    default:
        logError(_logger, "Unkwnon iterator type: %d", iterateStatement->type);
        break;
    }
    _output(0, ";\n");
}

/**
 * Generates an if statement.
 */
static void _generateIfStatement(const unsigned int indentationLevel, IfStatement * ifStatement) {
    _output(indentationLevel, "if (");
    _generateExpression(0, ifStatement->condition);
    _output(0, ") ");
    _generateBlock(0, ifStatement->blockIf);

    if (ifStatement->type == IF_ELSE_TYPE) {
        _output(2, " else ");
        _generateBlock(0, ifStatement->blockElse);
    }
}

/**
 * Generates a for statement.
 */
static void _generateForStatement(const unsigned int indentationLevel, ForStatement * forStatement) {
    _output(indentationLevel, "for (int %s = ", forStatement->varName);
    _generateExpression(0, forStatement->range->expressionLeft);
    _output(0, "; %s < ", forStatement->varName);
    _generateExpression(0, forStatement->range->expressionRight);
    _output(0, "; %s++) ", forStatement->varName);
    _generateBlock(0, forStatement->block);
}

static void _generateBlock(const unsigned int indentationLevel, Block * block){
    _output(indentationLevel, "{\n");
    _generateStatementList(block->statements);
    _output(0, "\n");
    _output(2, "}\n");
}

/**
 * Generates a function call.
 */
static void _generateFunctionCall(const unsigned int indentationLevel, FunctionCall * functionCall) {
    _output(indentationLevel, "%s.", functionCall->varName);

    switch (functionCall->type) {
        case INSERT_CALL:
            _output(0, "insert(");
            _generateExpression(0, functionCall->expression);
            _output(0, ")");
            break;
        case REMOVE_CALL:
            _output(0, "remove(");
            _generateExpression(0, functionCall->expression);
            _output(0, ")");
            break;
        case INCLUDES_CALL:
            _output(0, "includes(");
            _generateExpression(0, functionCall->expression);
            _output(0, ")");
            break;
        case HEIGHT_CALL:
            _output(0, "height()");
            break;
        case DEPTH_CALL:
            _output(0, "depth(");
            _generateExpression(0, functionCall->expression);
            _output(0, ")");
            break;
        case CALCULATE_CALL:
            _output(0, "calculate(\"");
            _generateExpression(1, functionCall->expression);
            _output(0, "\")");
            break;
        case VISUALIZE_CALL:
            _output(0, "visualize()");
            break;
        case ADD_CALL:
            _output(indentationLevel, "add(");
            _generateExpression(indentationLevel, functionCall->expression);
            _output(indentationLevel, ")");
           break;
		 case SUB_CALL:
            _output(indentationLevel, "sub(");
            _generateExpression(indentationLevel, functionCall->expression);
            _output(indentationLevel, ")");
            break;
        default:
            logError(_logger, "Unknown function call type: %d", functionCall->type);
            break;
	}
    _output(0, ";\n");
}

// /** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
} 

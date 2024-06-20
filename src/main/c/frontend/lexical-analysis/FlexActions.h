#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token BracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token RBTreeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BSTreeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EXPTreeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token AVLTreeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token NotEqualComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EqualComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LessEqualComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LessComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token GreaterEqualComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token GreaterComparisonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token PunctuationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ForLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token InLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ToLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IfLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ElseLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token InOrderIterationOrderLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token PostOrderIterationOrderLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token PreOrderIterationOrderLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token IntTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ConstantTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BooleanTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token InsertLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token RemoveLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IncludesLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token HeightLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DepthLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CalculateLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token VisualizeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IterateLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token DeclarationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif

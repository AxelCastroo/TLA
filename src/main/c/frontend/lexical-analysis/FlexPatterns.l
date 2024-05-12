%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"%"                                 { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }

"{"                                 { return BracketLexemeAction(createLexicalAnalyzerContext()); }
"}"                                 { return BracketLexemeAction(createLexicalAnalyzerContext()); }

"RBTree"                            { return RBTreeLexemeAction(createLexicalAnalyzerContext()); }
"BSTree"                            { return BSTreeLexemeAction(createLexicalAnalyzerContext()); }
"EXPTree"                           { return EXPTreeLexemeAction(createLexicalAnalyzerContext()); }
"AVLTree"                           { return AVLTreeLexemeAction(createLexicalAnalyzerContext()); }

"!="                                { return NotEqualComparisonLexemeAction(createLexicalAnalyzerContext()); }
"=="                                { return EqualComparisonLexemeAction(createLexicalAnalyzerContext()); }
"<="                                { return LessEqualComparisonLexemeAction(createLexicalAnalyzerContext()); }
"<"                                 { return LessComparisonLexemeAction(createLexicalAnalyzerContext()); }
">="                                { return GreaterEqualComparisonLexemeAction(createLexicalAnalyzerContext()); }
">"                                 { return GreaterComparisonLexemeAction(createLexicalAnalyzerContext()); }

"&"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
"|"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
"!"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
","                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
"="                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }

"for"                               { return ForLexemeAction(createLexicalAnalyzerContext()); }
"in"                                { return InLexemeAction(createLexicalAnalyzerContext()); }
"to"                                { return ToLexemeAction(createLexicalAnalyzerContext()); }
"if"                                { return IfLexemeAction(createLexicalAnalyzerContext()); }
"else"                              { return ElseLexemeAction(createLexicalAnalyzerContext()); }

"in-order"                          { return InOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 
"post-order"                        { return PostOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 
"pre-order"                         { return PreOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 

"Int"                               { return IntTypeLexemeAction(createLexicalAnalyzerContext()); }
"String"                            { return StringTypeLexemeAction(createLexicalAnalyzerContext()); }
"Constant"                          { return ConstantTypeLexemeAction(createLexicalAnalyzerContext()); }
"Boolean"                           { return BooleanTypeLexemeAction(createLexicalAnalyzerContext()); }

"insert"                            { return InsertLexemeAction(createLexicalAnalyzerContext()); }
"remove"                            { return RemoveLexemeAction(createLexicalAnalyzerContext()); }
"includes"                          { return IncludesLexemeAction(createLexicalAnalyzerContext()); }
"height"                            { return HeightLexemeAction(createLexicalAnalyzerContext()); }
"depth"                             { return DepthLexemeAction(createLexicalAnalyzerContext()); }
"calculate"                         { return CalculateLexemeAction(createLexicalAnalyzerContext()); }
"visualize"                         { return VisualizeLexemeAction(createLexicalAnalyzerContext()); }
"iterate"                           { return IterateLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
[A-Za-z][A-Za-z0-9]*                { return DeclarationLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"

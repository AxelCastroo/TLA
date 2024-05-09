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

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }

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

","                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
";"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }
"="                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext()); }

"For"                               { return LoopLexemeAction(createLexicalAnalyzerContext()); }

"in-order"                          { return InOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 
"post-order"                        { return PostOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 
"pre-order"                         { return PreOrderIterationOrderLexemeAction(createLexicalAnalyzerContext()); } 

"Int"                               { return IntTypeLexemeAction(createLexicalAnalyzerContext()); }
"Constant"                          { return ConstantTypeLexemeAction(createLexicalAnalyzerContext()); }
"Boolean"                           { return BooleanTypeLexemeAction(createLexicalAnalyzerContext()); }

"height"                            { return HeightLexemeAction(createLexicalAnalyzerContext()); }
"includes"                          { return IncludesLexemeAction(createLexicalAnalyzerContext()); }
"insert"                            { return InsertLexemeAction(createLexicalAnalyzerContext()); }
"remove"                            { return RemoveLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"

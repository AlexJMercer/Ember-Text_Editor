
#include "SyntaxHighlighter.hpp"
#include <QtDebug>



SyntaxHighLighter::SyntaxHighLighter(QTextDocument* parent)
	: QSyntaxHighlighter(parent)
{
	keywordFormat.setForeground(Qt::cyan);
	keywordFormat.setFontWeight(QFont::Bold);

	classFormat.setForeground(Qt::magenta);
	classFormat.setFontWeight(QFont::Bold);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(QColor::fromRgb(255, 165, 0));

	quoteFormat.setForeground(Qt::darkGreen);

	inlineCommentFormat.setForeground(Qt::darkGreen);

	blockCommentFormat.setForeground(Qt::darkGreen);
}



void SyntaxHighLighter::addKeywords(QStringList keywords)
{
	foreach(const QString &keyword, keywords)
	{
		addRule(QRegularExpression(keyword), keywordFormat);
	}
}



void SyntaxHighLighter::addRule(QRegularExpression pattern, QTextCharFormat format)
{
	HighlightingRule rule;
	rule.pattern = pattern;
	rule.format = format;
	rules.append(rule);
}



void SyntaxHighLighter::highlightBlock(const QString& text)
{
	foreach(const HighlightingRule & rule, rules)
	{
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

		while (matchIterator.hasNext())
		{
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}

	setCurrentBlockState(BlockState::NotInComment);
	highlightMultilineComments(text);
}


void SyntaxHighLighter::highlightMultilineComments(const QString& text)
{
	int startIndex = 0;

	if (previousBlockState() != BlockState::InComment)
	{
		startIndex = text.indexOf(blockCommentStart);
	}


	while (startIndex >= 0)
	{
		QRegularExpressionMatch match = blockCommentEnd.match(text, startIndex);

		int endIndex = match.capturedStart();
		int commentLen = 0;

		if (endIndex == -1)
		{
			setCurrentBlockState(BlockState::InComment);
			commentLen = text.length() - startIndex;
		}
		else
		{
			commentLen = endIndex - startIndex + match.capturedLength();
		}

		setFormat(startIndex, commentLen, blockCommentFormat);
		startIndex = text.indexOf(blockCommentStart, startIndex + commentLen);
	}
}




#ifndef SYNTAXHIGHLIGHTER_H_
#define SYNTAXHIGHLIGHTER_H_


#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>



class SyntaxHighLighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:

	SyntaxHighLighter(QTextDocument* parent = nullptr);

	virtual void addKeywords(QStringList keywords);
	virtual void addRule(QRegularExpression pattern, QTextCharFormat format);

	QChar getCodeBlockStartDelimiter() const { return codeBlockStart; }
	QChar getCodeBlockEndDelimiter() const { return codeBlockEnd; }



protected:
	
	virtual void highlightBlock(const QString& text) override;
	virtual void highlightMultilineComments(const QString& text);

	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};

	enum BlockState
	{
		NotInComment = 0,
		InComment = 1
	};


	QVector<HighlightingRule> rules;

	QRegularExpression blockCommentStart;
	QRegularExpression blockCommentEnd;


	QChar codeBlockStart;
	QChar codeBlockEnd;


	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat inlineCommentFormat;
	QTextCharFormat blockCommentFormat;
	QTextCharFormat quoteFormat;
	QTextCharFormat functionFormat;

};



#endif // !SYNTAXHIGHLIGHTER_H_
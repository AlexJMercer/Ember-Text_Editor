#ifndef CODE_EDITOR_H_
#define CODE_EDITOR_H_

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

#include "SyntaxHighlighter.hpp"
#include "Language.hpp"


using namespace Language;

class LineNumberArea;
class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget* parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();

	void setLanguage(Lang lang);
	inline Lang getLanguage() const { return lang; }

	~CodeEditor();

protected:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* object, QEvent* event) override;


private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);


private:
	QWidget* lineNumberArea;

	Lang lang;
	SyntaxHighLighter *highlighter;

	SyntaxHighLighter *generateHighlighter(Lang lang);
	

};

#endif // !CODE_EDITOR_H_

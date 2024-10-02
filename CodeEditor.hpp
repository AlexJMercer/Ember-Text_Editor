#ifndef CODE_EDITOR_H_
#define CODE_EDITOR_H_

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>


class LineNumberArea;
class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget* parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();


protected:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* object, QEvent* event) override;


private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);


private:
	QWidget* lineNumberArea;

};

#endif // !CODE_EDITOR_H_

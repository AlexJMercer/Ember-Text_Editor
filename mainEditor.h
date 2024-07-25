#ifndef EDITOR_H_
#define EDITOR_H_

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QFont>
#include <QRect>

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor(QWidget* parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent* event);
	//int getLineNumber();



protected:
	void resizeEvent(QResizeEvent* event) override;



private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void updateLineNumberArea(const QRect& rect, int dy);
	void highlightCurrentLine();


private:
	QWidget* lineNumberArea;


	int lineNumberAreaWidth() const;
};



#endif
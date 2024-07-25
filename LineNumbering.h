#ifndef LINENUMBER_H_
#define LINENUMBER_H_


#include "mainEditor.h"
//#include <QWidget>
//#include <QPlainTextEdit>


class LineNumberArea : public QWidget
{
public:
	LineNumberArea(QPlainTextEdit* editor) : QWidget(editor), textEdit(textEdit) { }

	QSize sizeHint() const;


protected:
	void paintEvent(QPaintEvent* event);


private:
	Editor *textEdit;

	int lineNumberAreaWidth() const
	{
		int digits = 1;
		int max = qMax(1, textEdit->document()->blockCount());
		while (max >= 10) {
			max /= 10;
			++digits;
		}
		int space = 3 + textEdit->fontMetrics().horizontalAdvance('9') * digits;
		return space;
	}
};




#endif
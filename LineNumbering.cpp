
#include "LineNumbering.h"


QSize LineNumberArea::sizeHint() const
{
	return QSize(lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event)
{
	textEdit->lineNumberAreaPaintEvent(event);
}
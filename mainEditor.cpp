#include "mainEditor.h"
#include "LineNumbering.h"

/*
 * Default Constructor for Editor creation with the QPlainTextEdit Widget
 */

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);

	connect(this, &Editor::blockCountChanged, this, &Editor::updateLineNumberAreaWidth);
	connect(this, &Editor::updateRequest, this, &Editor::updateLineNumberArea);
	connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);


	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}


void Editor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}



//int Editor::getLineNumber()
//{
//	int digits = 1;
//	int max = qMax(1, blockCount());
//	while (max >= 10) {
//		max /= 10;
//		++digits;
//	}
//
//	int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
//
//	return space;
//}

void Editor::resizeEvent(QResizeEvent* event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void Editor::updateLineNumberArea(const QRect& rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}



void Editor::highlightCurrentLine() 
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::darkGray).lighter(150);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);

		selection.cursor = textCursor();
		selection.cursor.clearSelection();

		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

int Editor::lineNumberAreaWidth() const
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}
	int space = 3 + fontMetrics().horizontalAdvance('9') * digits;
	return space;
}
#include "CodeEditor.hpp"
#include "LineNumberArea.hpp"

#include "SyntaxHighlight/CHighlighter.hpp"
#include "SyntaxHighlight/CPPHighlighter.hpp"
#include "SyntaxHighlight/PythonHighlighter.hpp"
#include "SyntaxHighlight/JavaHighlighter.hpp"


CodeEditor::CodeEditor(QWidget* parent)
	: QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);
    setLanguage(Lang::None);

	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(lineNumberArea);

    QColor color(Qt::black);
    color.setAlpha(60);

    painter.fillRect(event->rect(), color);

    QTextBlock block = firstVisibleBlock();

    int blockNumber = block.blockNumber();
    int top = qRound(
        blockBoundingGeometry(block).translated(contentOffset()).top()
    );

    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            if (blockNumber == textCursor().blockNumber())
                painter.setPen(Qt::cyan);
            else
                painter.setPen(Qt::white);


            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}


int CodeEditor::lineNumberAreaWidth()
{
    int digits = 3; // Minimum width of 3 numbers

    int max = qMax(1, blockCount());
    if (max > 999) {
        while (max >= 10) {
            max /= 10;
            ++digits;
        }
    }

    int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}


void CodeEditor::resizeEvent(QResizeEvent* event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect contentRect = contentsRect();
	lineNumberArea->setGeometry(QRect(contentRect.left(), contentRect.top(), lineNumberAreaWidth(), contentRect.height()));
}



bool CodeEditor::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        QTextCursor cursor = textCursor();
        switch (keyEvent->key()) {
        case Qt::Key_ParenLeft:
            insertPlainText("()");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_BracketLeft:
            insertPlainText("[]");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_BraceLeft:
            insertPlainText("{}");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_QuoteDbl:
            insertPlainText("\"\"");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_Apostrophe:
            insertPlainText("''");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        default:
            break;
        }
    }
    return QPlainTextEdit::eventFilter(object, event);
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
		QTextEdit::ExtraSelection selection;

        QColor lineColor(Qt::darkGray);
        lineColor.setAlpha(20);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		
        selection.cursor = textCursor();
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }

	setExtraSelections(extraSelections);
}



void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}



SyntaxHighLighter* CodeEditor::generateHighlighter(Lang lang)
{
    QTextDocument* doc = document();

    switch (lang)
    {
        case (Lang::C):
		    return new CHighLighter(doc);
        case (Lang::CPP):
		    return new CPPHighLighter(doc);
        case (Lang::Python):
		    return new PythonHighLighter(doc);
        case (Lang::Java):
		    return new JavaHighLighter(doc);
        default:
            return nullptr;
    }
 
}



void CodeEditor::updateLineNumberAreaWidth(int newBlockCount)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}




void CodeEditor::setLanguage(Lang lang)
{
	if (lang == this->lang)
		return;

	this->lang = lang;
	this->highlighter = generateHighlighter(lang);
}
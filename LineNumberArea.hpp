#ifndef LINENUMBERAREA_H_
#define LINENUMBERAREA_H_


#include <QWidget>
#include "CodeEditor.hpp"



class CodeEditor;
class LineNumberArea : public QWidget
{
public:
	LineNumberArea(CodeEditor* editor);
	QSize sizeHint() const override;


protected:
	void paintEvent(QPaintEvent* event) override;


private:
	CodeEditor *codeEditor;
};



#endif // !LINENUMBERAREA_H_

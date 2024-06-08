#ifndef EDITOR_H_
#define EDITOR_H_

#include <QtWidgets/QPlainTextEdit>
#include <QFont>

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor(QWidget* parent = nullptr);
};



#endif
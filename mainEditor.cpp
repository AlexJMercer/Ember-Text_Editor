#include "mainEditor.h"

/*
 * Default Constructor for Editor creation with the QPlainTextEdit Widget
 */

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent)
{
	setFont(QFont("Consolas", 12));

}
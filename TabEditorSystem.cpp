#include "TabEditorSystem.h"


/* 
 * Initialize with single tab
*/
TabEditor::TabEditor(QWidget* parent) : QTabWidget(parent)
{
	addNewTab(new Editor());
}


// Add a new Tab
void TabEditor::addNewTab(Editor* tab)
{
	QTabWidget::addTab(tab, QString("New File"));
	setCurrentWidget(tab);
}
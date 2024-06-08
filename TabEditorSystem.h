#ifndef TAB_EDITOR_H
#define TAB_EDITOR_H

#include <QtWidgets/QTabWidget>
#include "mainEditor.h"




class TabEditor : public QTabWidget
{
	Q_OBJECT

public:
	TabEditor(QWidget* parent = nullptr);
	void addNewTab(Editor* tab);

};



#endif
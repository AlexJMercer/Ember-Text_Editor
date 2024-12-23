#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "CodeEditor.hpp"
#include "Language.hpp"


#include "ui_EmberTextEditor.h"


QT_BEGIN_NAMESPACE
namespace Ui {
	class EmberTextEditor;
}
QT_END_NAMESPACE

class EmberTextEditor : public QMainWindow
{
    Q_OBJECT


public:
    EmberTextEditor(QWidget *parent = nullptr);


    ~EmberTextEditor();

private:
    Ui::EmberTextEditorClass* ui;
	CodeEditor* codeEditor;


    QString windowName = "Ember Text Editor";


	QString currentFileName;
	QFileInfo currFileInfo;


	void initEditor();
	void updateLineNumberLabel();


	void selectLanguageFromExtension();
	void setProgrammingLanguage(Lang lang);

	
	void updateWindowNameAndUnsaved();

private slots:
	void onAction_NewFile();
	void newFile();

	void onAction_OpenFile();
	
	void onAction_SaveFile();
	bool saveFile(QString &filePath);
	
	void onAction_SaveAsFile();
	bool saveFileAs(QString &filePath);
	
	void onAction_Exit();

	void changeOpacity();


};


#endif // MAINWINDOW_H
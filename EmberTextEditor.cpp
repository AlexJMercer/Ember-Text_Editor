#include "EmberTextEditor.hpp"

#include <QApplication>




EmberTextEditor::EmberTextEditor(QWidget* parent)
	: QMainWindow(parent),
	  ui(new Ui::EmberTextEditorClass()),
	  textChanged(false)
{
	
	ui->setupUi(this);

	initEditor();

	connect(ui->newBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_NewFile);
	connect(ui->openBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_OpenFile);
	connect(ui->saveBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_SaveFile);
	connect(ui->saveAsBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_SaveAsFile);
	connect(ui->exitBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_Exit);

	connect(codeEditor, &CodeEditor::textChanged, this, &EmberTextEditor::onTextChange);

	//connect(ui->actionCopy, &QAction::triggered, this, &EmberTextEditor::copyText);
	//connect(ui->actionCut, &QAction::triggered, this, &EmberTextEditor::cutText);
	//connect(ui->actionPaste, &QAction::triggered, this, &EmberTextEditor::pasteText);
	//connect(ui->actionUndo, &QAction::triggered, this, &EmberTextEditor::undoText);
	//connect(ui->actionRedo, &QAction::triggered, this, &EmberTextEditor::redoText);
	//connect(ui->actionAbout, &QAction::triggered, this, &EmberTextEditor::aboutApp);
	//connect(ui->actionAbout_Qt, &QAction::triggered, this, &EmberTextEditor::aboutQt);

	// Make the codeEditor bind to QPlainTextEdit
	
}




// Class Destructor
EmberTextEditor::~EmberTextEditor()
{
	delete ui;
}





/*
 * Initialize the Editor
 */

void EmberTextEditor::initEditor()
{
	setWindowTitle(windowName + " - Untitled");
	//setWindowIcon(QIcon(":/resources/mainIcon.png"));

	currentFileName = "untitled.txt";

	codeEditor = new CodeEditor(this);

	// Set Font Properties
	QFont textFont("Consolas", 14);
	textFont.setPointSize(15);
	codeEditor->setFont(textFont);

}






/*
 * Slots for Buttons:
 * 
 * New
 * Open 
 * Save
 * Save As
 * Exit
 * 
*/


void EmberTextEditor::onAction_NewFile()
{
	newFile();
}


void EmberTextEditor::onAction_OpenFile()
{
	QString filter = "Supported Files (*.txt *.cpp *.c *.hpp *.h *.py *.html *.css *.js)";
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", "", filter);

	QFileInfo fileInfo(filePath);
	QString folderPath = fileInfo.absolutePath();

	if (!filePath.isEmpty())
	{
		currFileInfo = QFileInfo(filePath);
		currentFileName = fileInfo.fileName();

		setWindowTitle(windowName + " - " + currentFileName);

		QFile file(filePath);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			QString content = in.readAll();
			codeEditor->setPlainText(content);
			file.close();
		}
		else
			QMessageBox::warning(nullptr, "Error", "Could not open file.");
	}
	else
		QMessageBox::warning(nullptr, "Error", "Could not open file.");
}


void EmberTextEditor::onAction_SaveFile()
{
	QString path(currFileInfo.absoluteFilePath());
	saveFile(path);
}


void EmberTextEditor::onAction_SaveAsFile()
{
	QString path(currFileInfo.absoluteFilePath());
	saveFileAs(path);
}


void EmberTextEditor::onAction_Exit()
{

}


/*
*****************************************************************************
*
*					File Handling Functions
*
*****************************************************************************
*/


void EmberTextEditor::newFile()
{
	if (textChanged)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle("Warning");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::No);

		int result = msgBox.exec();

		if (result == QMessageBox::Cancel)
		{
			return;
		}
		else if (result == QMessageBox::Yes)
		{
			QString path = currFileInfo.absoluteFilePath();
			saveFile(path);
		}
	}

	codeEditor->setPlainText("");
	currentFileName = "untitled.txt";

	setWindowTitle(windowName + " - " + currentFileName);
	textChanged = false;
}


bool EmberTextEditor::saveFile(QString &filePath)
{
	bool savedAs = false;

	QString content = codeEditor->toPlainText();
	
	if (filePath.isEmpty())
	{
		if (!saveFileAs(filePath))
			return false;
		else
			savedAs = true;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(nullptr, "Error", "Could not save changes to file.");
		return false;
	}

	if (!savedAs)
	{
		QTextStream out(&file);
		out << content;

		currFileInfo = QFileInfo(filePath);
		currentFileName = currFileInfo.fileName();

		setWindowTitle(windowName + " - " + currentFileName);
	}

	file.close();
	textChanged = false;

	return true;
}


bool EmberTextEditor::saveFileAs(QString &filePath)
{
	QString filter = "Supported Files (*.txt *.cpp *.c *.hpp *.h *.py *.html *.css *.js)";
	filePath = QFileDialog::getSaveFileName(nullptr, "Save File", "", filter);

	if (filePath.isEmpty())
		return false;	

	currFileInfo = QFileInfo(filePath);
	currentFileName = currFileInfo.fileName();
	setWindowTitle(windowName + " - " + currentFileName);

	QString content = codeEditor->toPlainText();
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(nullptr, "Error", "Could not save changes to file.");
		return false;
	}

	QTextStream out(&file);
	out << content;
	textChanged = false;

	return true;
}


void EmberTextEditor::onTextChange()
{
	textChanged = true;
	setWindowTitle(windowName + " - " + currentFileName + "*");
}
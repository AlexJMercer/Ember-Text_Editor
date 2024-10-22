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

	connect(codeEditor, &QPlainTextEdit::cursorPositionChanged, this, &EmberTextEditor::updateLineNumberLabel);


	//connect(ui->actionCopy, &QAction::triggered, this, &EmberTextEditor::copyText);
	//connect(ui->actionCut, &QAction::triggered, this, &EmberTextEditor::cutText);
	//connect(ui->actionPaste, &QAction::triggered, this, &EmberTextEditor::pasteText);
	//connect(ui->actionUndo, &QAction::triggered, this, &EmberTextEditor::undoText);
	//connect(ui->actionRedo, &QAction::triggered, this, &EmberTextEditor::redoText);
	//connect(ui->actionAbout, &QAction::triggered, this, &EmberTextEditor::aboutApp);
	//connect(ui->actionAbout_Qt, &QAction::triggered, this, &EmberTextEditor::aboutQt);

	
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

	codeEditor = ui->plainTextEdit;

	// Scale the codeEditor to the size of the plainTextEdit
	//codeEditor->setGeometry(ui->plainTextEdit->geometry());

	
	// Set Font Properties
	QFont textFont("Consolas", 12);
	textFont.setPointSize(12);
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

/// <summary>
/// @brief Open a file from the file system
/// @details User can click on the Open File button to choose a file 
///			 from the file system and display its content in the code editor
/// </summary>
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

	selectLanguageFromExtension();
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
	if (textChanged)
	{
        int res = QMessageBox::warning(this, "Unsaved Changes", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		
		if (res == QMessageBox::No)
			onAction_SaveFile();
	}

	QApplication::quit();
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
		msgBox.setText("Do you want to save changes to " + currentFileName + "?");
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
	selectLanguageFromExtension();

	return true;
}


bool EmberTextEditor::saveFileAs(QString &filePath)
{
	QString filter = "Supported Files (*.txt *.cpp *.c *.hpp *.h *.py *.java)";
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


void EmberTextEditor::updateLineNumberLabel()
{
	int currentLine = codeEditor->textCursor().blockNumber() + 1;

	// Update the label
	QString lineLabel = QString("%1 of %2").arg(currentLine).arg(codeEditor->document()->blockCount());
    ui->labelLineNumber->setText(lineLabel);
}



void EmberTextEditor::selectLanguageFromExtension()
{
	QString ext = currFileInfo.suffix();
	
	if (ext == "c" || ext == "h")
		setProgrammingLanguage(Lang::C);
	
	else if (ext == "cpp" || ext == "hpp")
		setProgrammingLanguage(Lang::CPP);
	
	else if (ext == "java")
		setProgrammingLanguage(Lang::Java);
	
	else if (ext == "py")
		setProgrammingLanguage(Lang::Python);
	
	else
		setProgrammingLanguage(Lang::None);
}



void EmberTextEditor::setProgrammingLanguage(Lang lang)
{
	if (lang == codeEditor->getLanguage())
		return;

	codeEditor->setLanguage(lang);
}
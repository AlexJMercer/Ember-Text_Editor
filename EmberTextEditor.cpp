#include "EmberTextEditor.hpp"

#include <QApplication>




EmberTextEditor::EmberTextEditor(QWidget* parent)
	: QMainWindow(parent),
	  ui(new Ui::EmberTextEditorClass())
{
	
	ui->setupUi(this);

	initEditor();

	connect(ui->newBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_NewFile);
	connect(ui->openBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_OpenFile);
	connect(ui->saveBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_SaveFile);
	connect(ui->saveAsBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_SaveAsFile);
	connect(ui->exitBtn, &QPushButton::clicked, this, &EmberTextEditor::onAction_Exit);

    connect(codeEditor, &QPlainTextEdit::textChanged, this, &EmberTextEditor::updateWindowNameAndUnsaved);

	connect(codeEditor, &QPlainTextEdit::cursorPositionChanged, this, &EmberTextEditor::updateLineNumberLabel);

	connect(ui->opacitySlider, &QSlider::valueChanged, this, &EmberTextEditor::changeOpacity);
	
	ui->opacitySlider->setMinimum(10);
	ui->opacitySlider->setMaximum(100);
	ui->opacitySlider->setValue(100);
	
	ui->opacitySlider->setStyleSheet(
		"QSlider::handle:horizontal {"
		"    background: #2d2d2d;"			// Change handle color to dark cyan
		"    width: 10px;"					// Set handle width
		"    margin: -6px 0;"				// Set handle margin
		"    border-radius: 6px;"			// Set handle border radius
		"    border: 1px solid #000000;    "
		"}"
		"    QSlider::handle:horizontal:hover{		"
		"	 background-color: #00bfbf;		"// Change handle color to dark cyan
		"}"
		"QSlider::groove:horizontal {"
		"    background: cyan;"				// Change groove color to cyan
		"    height: 1px;"					// Set groove height
		"    margin: 0;"					// Default margin
		"}"
	);


	
}




// Class Destructor
EmberTextEditor::~EmberTextEditor()
{
	delete ui;
	delete codeEditor;
}





/*
 * Initialize the Editor
 */

void EmberTextEditor::initEditor()
{
	setWindowTitle(windowName + " - Untitled");
	setWindowIcon(QIcon(":/EmberTextEditor/icons/flames.png"));

	currentFileName = "untitled.txt";

	codeEditor = ui->plainTextEdit;
	
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

///
/// @brief Open a file from the file system
/// @details User can click on the Open File button to choose a file 
///			 from the file system and display its content in the code editor
///
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
		}
		else
			QMessageBox::warning(nullptr, "Error", "Could not open file.");
		
		file.close();
	}
	else
		QMessageBox::warning(nullptr, "Error", "Could not open file.");

	selectLanguageFromExtension();
	codeEditor->setModifiedState(false);
	updateWindowNameAndUnsaved();
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
	if (codeEditor->isUnsaved())
	{
        int res = QMessageBox::warning(this, "Unsaved Changes", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		
		if (res == QMessageBox::No)
			onAction_SaveFile();
		else if (res == QMessageBox::Cancel)
			return;
	}

	QApplication::quit();
}

void EmberTextEditor::changeOpacity()
{
	int value = ui->opacitySlider->value();
	float opacity = value / 100.0f;
	
	setWindowOpacity(opacity);
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
	// Check if there are unsaved changes
	if (codeEditor->isUnsaved())
	{
		int res = QMessageBox::warning(this, "Unsaved Changes", "Are you sure you want to create a new file?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (res == QMessageBox::No)
			onAction_SaveFile();
		else if (res == QMessageBox::Cancel)
			return;
	}

	codeEditor->clear();
	currentFileName = "Untitled";

	currFileInfo = QFileInfo();
		
	codeEditor->setModifiedState(false);
	updateWindowNameAndUnsaved();
	setProgrammingLanguage(Lang::None);
}


bool EmberTextEditor::saveFile(QString &filePath)
{
	bool savedAs = false;

	if (filePath.isEmpty())
	{
		if (!saveFileAs(filePath))
			return false;
		else
			savedAs = true;
	}
	
	QFile file(filePath);

	if (!savedAs) 
	{
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(nullptr, "Error", "Could not save changes to file.");
			return false;
		}

		QTextStream out(&file);
		QString content = codeEditor->toPlainText();
		out << content;

		currFileInfo = QFileInfo(filePath);
		currentFileName = currFileInfo.fileName();
	
		codeEditor->setModifiedState(false);
		updateWindowNameAndUnsaved();
		selectLanguageFromExtension();
	
	}
	file.close();

	return true;
}


bool EmberTextEditor::saveFileAs(QString &filePath)
{
	filePath = QFileDialog::getSaveFileName(nullptr, "Save File As", "", "Supported Files (*.txt *.cpp *.c *.hpp *.h *.py *.java)");

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
	
	file.close();
	
	codeEditor->setModifiedState(false);
	updateWindowNameAndUnsaved();
	
	return true;
}


void EmberTextEditor::updateWindowNameAndUnsaved()
{
	if (codeEditor->isUnsaved())
		setWindowTitle(windowName + " - " + currentFileName + "*");
	else
		setWindowTitle(windowName + " - " + currentFileName);
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




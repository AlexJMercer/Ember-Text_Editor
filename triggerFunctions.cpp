#include "EmberTextEditor.h"


/*
 * Functions declarations of Close and Minimize Buttons
 */

void EmberTextEditor::onAction_CloseWindow() {

	if (saveCurrentFile() == -1) { return; }
	

	QApplication::quit();
}

void EmberTextEditor::onAction_MinimizeButton() {
	QMainWindow::setWindowState(Qt::WindowMinimized);
}


/*
* Function declarations of Buttons when triggered in UI
* 
* Functions Declarations in "EmberTextEditor.cpp"
*/

void EmberTextEditor::onAction_NewTriggered() 
{	
	saveCurrentFile();

	ui->plainTextEdit->clear();
	currentFileName.clear();
	ui->fileNameLabel->clear();
	ui->plainTextEdit->document()->setModified(false);

	updateWindowTitle();
}

void EmberTextEditor::onAction_OpenTriggered() 
{
	saveCurrentFile();

	QString fileName = QFileDialog::getOpenFileName(
		this,
		"Open a File",
		"",
		"Text Files (*.txt)"
	);

	if (!fileName.isEmpty())
	{
		QFile file(fileName);

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, "Error Opening File", "Could not open file.");
			return;
		}

		QTextStream inFile(&file);
		QString fileContent = inFile.readAll();
		ui->plainTextEdit->setPlainText(fileContent);
		ui->plainTextEdit->document()->setModified(false);

		currentFileName = fileName;

		updateWindowTitle();
	}
}

void EmberTextEditor::onAction_SaveTriggered()
{
	if (currentFileName.isEmpty())
	{
		onAction_SaveAsTriggered();
	}
	else
	{
		QFile file(currentFileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, "Error", "Could not Save File");
			return;
		}

		QTextStream outFile(&file);
		outFile << ui->plainTextEdit->toPlainText();
		ui->plainTextEdit->document()->setModified(false);
		updateWindowTitle();
	}
}

void EmberTextEditor::onAction_SaveAsTriggered()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, 
		"Save File As", 
		"", 
		"Text Files (*.txt)"
	);

	if (!fileName.isEmpty())
	{
		currentFileName = fileName;
		onAction_SaveTriggered();
		updateWindowTitle();
	}
}


// Custom Functions

void EmberTextEditor::updateWindowTitle()
{
	{
		if (currentFileName.isEmpty()) {
			ui->fileNameLabel->setText("Untitled");
		}
		else
		{
			QFileInfo fileInfo(currentFileName);
			ui->fileNameLabel->setText(fileInfo.fileName());
		}
	}
}

int EmberTextEditor::saveCurrentFile()
{
	{
		if (ui->plainTextEdit->document()->isModified())
		{
			QMessageBox::StandardButton reply;

			reply = QMessageBox::question(
				this,
				"Unsaved Changes",
				"Do you want to save your changes?",
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
			);

			if (reply == QMessageBox::Yes) {
				onAction_SaveTriggered();
				return 0;
			}
			else if (reply == QMessageBox::Cancel) {
				return -1;
			}
		}

		return 1;
	}
}

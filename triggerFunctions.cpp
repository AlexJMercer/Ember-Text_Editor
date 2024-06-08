#include "EmberTextEditor.h"

/*
 * Functions declarations of Close and Minimize Buttons
 */

void EmberTextEditor::onAction_CloseWindow() {
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

void EmberTextEditor::onAction_NewTriggered() {
	tabEditor->addNewTab(new Editor());
}

void EmberTextEditor::onAction_OpenTriggered() {

}
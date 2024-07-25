#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QEvent>
#include <QPoint>


#include "ui_EmberTextEditor.h"
#include "mainEditor.h"


class EmberTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    EmberTextEditor(QWidget *parent = nullptr);


    
    ~EmberTextEditor();

private:
    Ui::EmberTextEditorClass *ui;
    
    Editor *textEdit = nullptr;

    QString currentFileName;


    // Functions for Close & Minimize Buttons of Main Window
    void onAction_CloseWindow();
    void onAction_MinimizeButton();

    // Function Definitions for Buttons
    void onAction_NewTriggered();
    void onAction_OpenTriggered();
    void onAction_SaveTriggered();
    void onAction_SaveAsTriggered();


    // Custom Functions
    void updateWindowTitle();
    int saveCurrentFile();
    



    // Mouse Events for Dragging Frameless Window
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint curPos;
    QPoint newPos;

};

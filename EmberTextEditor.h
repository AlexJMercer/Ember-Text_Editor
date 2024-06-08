#pragma once

#include <QtWidgets/QMainWindow>
#include <QEvent>
#include <QPoint>


#include "ui_EmberTextEditor.h"
#include "mainEditor.h"
#include "TabEditorSystem.h"


class EmberTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    EmberTextEditor(QWidget *parent = nullptr);


    
    ~EmberTextEditor();

private:
    Ui::EmberTextEditorClass *ui;
    
    TabEditor *tabEditor;
    Editor *editor = nullptr;




    // Functions for Close & Minimize Buttons of Main Window
    void onAction_CloseWindow();
    void onAction_MinimizeButton();

    // Function Definitions for Buttons
    void onAction_NewTriggered();
    void onAction_OpenTriggered();


    // Mouse Events for Dragging Frameless Window
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint curPos;
    QPoint newPos;

};

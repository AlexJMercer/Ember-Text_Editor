#pragma once

#include <QtWidgets/QMainWindow>
#include <qevent.h>
#include <qpoint.h>


#include "ui_EmberTextEditor.h"


class EmberTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    EmberTextEditor(QWidget *parent = nullptr);
    ~EmberTextEditor();

private:
    Ui::EmberTextEditorClass ui;

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

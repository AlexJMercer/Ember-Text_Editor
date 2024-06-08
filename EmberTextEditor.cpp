#include "EmberTextEditor.h"

EmberTextEditor::EmberTextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmberTextEditorClass)
{
    ui->setupUi(this);

    // Connect Close and Minimize Buttons to their respective Functions Definitions
    connect(ui->closeMainWindowButton, &QPushButton::clicked, this, [this]() { onAction_CloseWindow(); });
    connect(ui->minimizeMainWinButton, &QPushButton::clicked, this, [this]() { onAction_MinimizeButton(); });


    // Connect Button to their respective Function Definitions
    connect(ui->NewFileButton, &QToolButton::clicked, this, [this]() { onAction_NewTriggered(); });
    connect(ui->OpenFileButton, &QToolButton::clicked, this, [this]() { onAction_OpenTriggered(); });
    
    // Frameless Window
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
    

    // Set up Tab Editor
    tabEditor = qobject_cast<TabEditor *>(ui->tabWidget);

}

EmberTextEditor::~EmberTextEditor()
{
    delete ui;

}


// Mouse Events
void EmberTextEditor::mousePressEvent(QMouseEvent *event) {
    curPos = event->globalPosition().toPoint();
}

void EmberTextEditor::mouseMoveEvent(QMouseEvent *event) {
    newPos = QPoint(event->globalPosition().toPoint() - curPos);
    move(x() + newPos.x(), y() + newPos.y());
    curPos = event->globalPosition().toPoint();
}
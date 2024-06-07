#include "EmberTextEditor.h"


#include <QtWidgets/QApplication>
#include <qstylefactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("fusion");

    EmberTextEditor window;
    window.show();
    
    return app.exec();
}

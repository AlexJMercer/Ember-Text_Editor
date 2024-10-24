#include "EmberTextEditor.hpp"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setStyle(QStyleFactory::create("fusion"));

    EmberTextEditor window;
    window.show();
    
    return app.exec();
}

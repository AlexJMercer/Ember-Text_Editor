#include "EmberTextEditor.hpp"


#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	QApplication::setStyle("Fusion");

    EmberTextEditor window;

    window.show();
    
    return app.exec();
}

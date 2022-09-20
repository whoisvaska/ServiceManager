#include "service_client_gui.h"
#include <QtWidgets/QApplication>
#include "qstylefactory.h"

int main(int argc, char *argv[])
{

    QApplication::setStyle(QStyleFactory::create("fusion"));

    QApplication a(argc, argv);
    ServiceClientGUI w;
    
    w.show();

    return a.exec();
}

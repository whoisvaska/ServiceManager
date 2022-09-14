#include "service_client_gui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

   

    QApplication a(argc, argv);
    ServiceClientGUI w;
    w.show();

    return a.exec();
}

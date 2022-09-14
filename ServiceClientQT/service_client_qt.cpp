#include "service_client_qt.h"
#include "../build-ServiceClientQT-Desktop_Qt_5_15_2_MSVC2019_32bit-Debug/ui_service_client_qt.h"

ServiceClientQT::ServiceClientQT(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServiceClientQT)
{
    static const QStringList HEADERS = QStringList()
            << "ServiceName"
            << "DisplayName"
            << "ServiceType"
            << "CurrentState"
            << "ControlsAccepted"
            << "Win32ExitCode"
            << "ServiceSpecificExitCode"
            << "CheckPoint"
            << "WaitHint";

    ui->setupUi(this);
}

ServiceClientQT::~ServiceClientQT()
{
    delete ui;
}



void ServiceClientQT::on_columnView_clicked(const QModelIndex &index)
{

}


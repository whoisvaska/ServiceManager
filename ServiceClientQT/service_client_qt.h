#ifndef SERVICE_CLIENT_QT_H
#define SERVICE_CLIENT_QT_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>

#include "../ServiceManager/server_type_info_h.h"
#include "../ServiceManager/server_type_info_i.c"


namespace Ui {
class ServiceClientQT;
}

class ServiceClientQT : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceClientQT(QWidget *parent = nullptr);
    ~ServiceClientQT();

private slots:

    void on_columnView_clicked(const QModelIndex &index);

private:
    Ui::ServiceClientQT *ui;

    IClassFactory* pICF;
    ICreateManager* pICreateManager;
    IManager* pIManager;

//    QTreeView* servicesTreeView;

//    QStandardItemModel* servicesModel;

    SAFEARRAY* servicesStatus;
};

#endif // SERVICE_CLIENT_QT_H

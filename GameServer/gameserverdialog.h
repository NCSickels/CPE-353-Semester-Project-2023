#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDebug>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql>
#include <QMap>
#include "mytcpserver.h"

namespace Ui
{
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    int netmaskToCIDR(QString netmask);

protected:
private:
    Ui::Dialog *ui;
    MyTCPServer *tcpServer;
    QSet<QTcpSocket *> clients;
    QMap<QTcpSocket*, QString> clientEntityMap;
    QSqlDatabase db;
    QSqlQueryModel qm;
    void initDatabase();
    void refreshTable();

private slots:
    void readClient();
    void incomingConnection(qintptr socketDescriptor);
};

#endif // DIALOG_H

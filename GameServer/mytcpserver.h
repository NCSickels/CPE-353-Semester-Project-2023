#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>

class MyTCPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTCPServer(QObject *parent = nullptr);
    ~MyTCPServer();
protected:
    void incomingConnection(qintptr socketDescriptor);
signals:
    void forwardData(qintptr msg);
};

#endif // MYTCPSERVER_H

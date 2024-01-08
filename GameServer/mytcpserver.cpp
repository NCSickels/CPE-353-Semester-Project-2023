#include "mytcpserver.h"

MyTCPServer::MyTCPServer(QObject *parent) : QTcpServer(parent)
{

}
MyTCPServer::~MyTCPServer() {

}

void MyTCPServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "Ayyyy Connection got";
    emit forwardData(socketDescriptor);
    QTcpServer::incomingConnection(socketDescriptor);
}

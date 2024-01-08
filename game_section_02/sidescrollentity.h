#ifndef SIDESCROLLENTITY_H
#define SIDESCROLLENTITY_H

//DEI
#include <QtDebug>
#include <QTcpSocket>
#include "entityanimation.h"
#include "defs.h"
static QByteArray entity_response_buffer;
class SideScrollEntity : public QObject
{
    Q_OBJECT
public:
    ~SideScrollEntity();
    // Constructor for when We spawn New
    explicit SideScrollEntity(QTcpSocket *connection,coordinates entryPoint, QGraphicsScene* graphicsScene, int HP, spriteType type, QObject* parent = nullptr);
    // Constructor for when We catch up
    SideScrollEntity(QTcpSocket* connection, QByteArray ID, QGraphicsScene* graphicsScene, QObject* parent = nullptr);


    void setLocation(coordinates loc);
    void setScreenPos(qreal x, qreal y);

    void setHP(int);
    int  getHP();

    void initializeToScene();

    EntityAnimation* getGraphicsItem() const;
    coordinates getLocation() const;
    QByteArray ID = "0";  // Made public

private:
    bool debug = true; // For the time being leaving debug to true

    coordinates loc;
    spriteType type;

    QString expecting;

    QTcpSocket* connection;
    EntityAnimation* animation = nullptr;
    QGraphicsScene* scene = nullptr;

    int hitpoints;

    // Request Server to fill us in on what our info is.
    void requestUpdate();

    bool inScene = false;

private slots:
    void recievedData();
};

#endif // SIDESCROLLENTITY_H

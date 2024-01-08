#ifndef PLAYER_H
#define PLAYER_H


#include <QGraphicsScene>
#include <QtMath>
#include <QTcpSocket>

#include "sidescrollentity.h"


class Player : public SideScrollEntity
{
public:
    Player(QTcpSocket* conn, spriteType avatar, QGraphicsScene* scene, qreal speed = 20);

    void SetMoveDirection(coordinates newVelocity);
    void Move(coordinates v);

    EntityAnimation* GetGraphicsItem() const;
    coordinates GetVelocity() const;

private:
    coordinates LimitVelocity(coordinates v, qreal maximumSpeed);

    coordinates velocity;
    qreal maxSpeed;
};


#endif // PLAYER_H

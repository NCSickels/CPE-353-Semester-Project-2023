#include "player.h"




Player::Player(QTcpSocket* conn, spriteType avatar, QGraphicsScene* scene, qreal speed) : SideScrollEntity(conn, coordinates {0, 0}, scene, 100, avatar)
{
    velocity.x = 0.0; velocity.y = 0.0;
    maxSpeed = speed;
}


void Player::SetMoveDirection(coordinates newVelocity)
{
    velocity = {newVelocity.x * maxSpeed, newVelocity.y * maxSpeed};

    velocity = LimitVelocity(velocity, maxSpeed);
}


void Player::Move(coordinates v)
{
    coordinates temp = this->getLocation();

    temp.x += v.x; temp.y -= v.y;

    this->setLocation(temp);
}


coordinates Player::LimitVelocity(coordinates v, qreal maximumSpeed)
{
    qreal magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2));

    if (magnitude > maximumSpeed)     // v has a greater magnitude than allowed
    {
        v.x = magnitude / sqrt(2) * v.x/abs(v.x);
        v.y = magnitude / sqrt(2) * v.y/abs(v.y);
    }

    return v;
}


coordinates Player::GetVelocity() const
{
    return velocity;
}

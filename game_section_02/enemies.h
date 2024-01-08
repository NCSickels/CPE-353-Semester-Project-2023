#ifndef ENEMIES_H
#define ENEMIES_H

#include "sidescrollentity.h"
#include "defs.h"


class Enemies : public SideScrollEntity
{
public:
    Enemies(QTcpSocket* conn, spriteType avatar, QGraphicsScene* scene);
};

#endif // ENEMIES_H


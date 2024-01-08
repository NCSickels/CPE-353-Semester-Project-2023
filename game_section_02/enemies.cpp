#include "enemies.h"


Enemies::Enemies(QTcpSocket* conn, spriteType avatar, QGraphicsScene* scene) : SideScrollEntity(conn, coordinates {0, 0}, scene, 100, avatar)
{

}

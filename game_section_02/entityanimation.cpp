#include "entityanimation.h"


EntityAnimation::EntityAnimation(QGraphicsItem *parent)
{
    coordinates i;
    i.x = 0;
    i.y = 0;
}


EntityAnimation::EntityAnimation(spriteType type, QGraphicsItem *parent) {

 thisSprite = type;          //set sprite type
 this->currentstate = Special;
 setState(Idle);             //set current animation loop
}


EntityAnimation::~EntityAnimation()
{

}


void EntityAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    frameIndex = (frameIndex + 1) % ANIMATION_CYCLE;
    if(thisSprite < 12)
    {
        painter->drawPixmap(-PCNPC_WIDTH/2, -PCNPC_HEIGHT/2, animation[frameIndex]);
    }
    else
    {
        painter->drawPixmap(-INVENTORY_ENT_SIZE/2, -INVENTORY_ENT_SIZE/2, animation[frameIndex]);
    }
    if (debug == 1)
    {
        painter->setPen(QPen(Qt::red));
        painter->drawRect(this->boundingRect());
        painter->setPen(QPen(Qt::green));
        if(thisSprite < 12)
        {
            painter->drawRect(-PCNPC_WIDTH/4, -PCNPC_HEIGHT/4, PCNPC_WIDTH/2, PCNPC_HEIGHT/2);
        }
        else
        {
            painter->drawRect(-INVENTORY_ENT_SIZE/4, -INVENTORY_ENT_SIZE/4, INVENTORY_ENT_SIZE/2, INVENTORY_ENT_SIZE/2);
        }
    }
}


QRectF EntityAnimation::boundingRect() const
{
    if(thisSprite < 12)
    {
        return QRectF(-PCNPC_WIDTH/2, -PCNPC_HEIGHT/2, PCNPC_WIDTH, PCNPC_HEIGHT);
    }
    else
    {
        return QRectF(-INVENTORY_ENT_SIZE/2, -INVENTORY_ENT_SIZE/2, INVENTORY_ENT_SIZE, INVENTORY_ENT_SIZE);
    }
}


QPainterPath EntityAnimation::shape() const
{
    QPainterPath path;
    if(thisSprite < 12)
    {
        path.addRect(-PCNPC_WIDTH/4, -PCNPC_HEIGHT/4, PCNPC_WIDTH/2, PCNPC_HEIGHT/2);
    }
    else
    {
        path.addRect(-INVENTORY_ENT_SIZE/4, -INVENTORY_ENT_SIZE/4, INVENTORY_ENT_SIZE/2, INVENTORY_ENT_SIZE/2);
    }
    return path;
}


void EntityAnimation::advance(int phase)
{
    if (phase == 0)
    {
        return;
    }

    //Compute Coordinates to move image here***
    this->update();
}


void EntityAnimation::setState(state newState) {
    if(this->currentstate != newState)
    {
        this->currentstate = newState;
        //start at first frame in animation
        frameIndex = 0;
        //Fill array with new graphics
        for(int f = 0; f < ANIMATION_CYCLE; f++)
        {
            //qDebug() << "loop: " <<f;
            QString path = ":/sprites/"+QString::number(thisSprite)+"/"+QString::number(f+currentstate)+".png";
            animation[f] = QPixmap(path);
            if(thisSprite < 12)
            {
                animation[f] = animation[f].scaled(PCNPC_WIDTH, PCNPC_HEIGHT);
            }
            else
            {
                animation[f] = animation[f].scaled(INVENTORY_ENT_SIZE, INVENTORY_ENT_SIZE);
            }
        }
    }


}


void EntityAnimation::setScreenPos(qreal x, qreal y)
{
    this->setPos(x, y);
}

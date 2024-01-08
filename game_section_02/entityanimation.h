#ifndef ENTITYANIMATION_H
#define ENTITYANIMATION_H


// DEI
#include <QtDebug>
#include "defs.h"
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QtGlobal>
#include <QObject>


const int PCNPC_HEIGHT = 104;
const int PCNPC_WIDTH = 80;
const int INVENTORY_ENT_SIZE = 64;
const int ANIMATION_CYCLE = 4;        //Frames per Animation
//const int NUM_IMAGES = 20;            // Total number of images in Pixmap

//Note that each sprite does not have access to all states
enum state {Idle = 0, Down = 4, Up = 8, Right = 12, Left = 16,
            DownAction = 20, UpAction = 24, RightAction = 28, LeftAction = 32,
            Special = 36};

enum spriteType {knight = 1, alchemist = 2, mage = 3, samurai = 4,                         //pc(Idle, Up, Down, Right, Left)                 NOT IMPLEMENTED: mage, samurai
                 slime = 5, skeleton = 6,                                                  //enemies(Idle, Up, Down, Right, Left)            NOT IMPLEMENTED: skeleton
                 wall = 7,                                                                 //walls                                           NOT IMPLEMENTED: all
                 turret = 8,                                                               //turrets                                         NOT IMPLEMENTED: all
                 resourceWood = 9, resourceStone = 10, resourceIron = 11,                  //resource points(Idle, Special(break))           NOT IMPLEMENTED: all
                 iconWood = 12, iconStone = 13, iconIron = 14,                             //inventory icons(Idle)
                 iconWoodFrameless = 15, iconStoneFrameless = 16, iconIronFrameless = 17,  //item icons(Idle)
                 itemFrame = 18};                                                          //gui elements(Idle)

const int TICK = 2000;                 //Standard Animation Tick Speed

class EntityAnimation : public QGraphicsItem
{
/*
    Entity Animation should update its location with the
*/

public:

    EntityAnimation(QGraphicsItem *parent = nullptr);                                                      //Default Constructor
    EntityAnimation(spriteType type, QGraphicsItem *parent = nullptr);            //Standard Constructor

    ~EntityAnimation() override;                                                     //Destructor

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void advance(int phase) override;

    void setState(state newState);
    void setScreenPos(qreal x, qreal y);
	
private:
    // For the time being leaving debug to true
    bool debug = true;

    // Constructor should  set relative coordinates based on
    // loc.getRelativecoordinates, then update them as needed.
    coordinates relCoords;
	
    // Initializing to default state
    state currentstate;
    int frameIndex;
    spriteType thisSprite;

    // Generate animation loop array
    QPixmap animation[ANIMATION_CYCLE];

    // Animation timer
    QTimer* animationTick;


    // QGraphicsScene
    QGraphicsScene* scene = nullptr;
};


#endif // ENTITYANIMATION_H

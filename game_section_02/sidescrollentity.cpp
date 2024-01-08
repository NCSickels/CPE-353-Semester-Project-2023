#include "sidescrollentity.h"

// Request Data Update from server
void SideScrollEntity::requestUpdate() {
    QByteArray cmd = "E|";
    cmd.append(this->ID);
    cmd.append("@");
    this->connection->write(cmd);
}


// Constructor for when We spawn New
SideScrollEntity::SideScrollEntity(QTcpSocket* connection,coordinates entryPoint, QGraphicsScene* graphicsScene, int HP, spriteType type, QObject* parent) : QObject (parent)
{
    this->connection = connection;

    connect(this->connection, &QTcpSocket::readyRead, this, &SideScrollEntity::recievedData);

    this->scene = graphicsScene;
    this->hitpoints = HP;
    this->type = type;
    this->loc = entryPoint;

    this->animation = new EntityAnimation(type);

    this->expecting = "EID";
    QByteArray response = "S|";
    QByteArray coord;
    coord.setNum(Idle);
    response.append(coord);
    response.append("|");
    coord.setNum(entryPoint.x);
    response.append( coord);
    response.append("|");
    coord.setNum(entryPoint.y);
    response.append( coord);
    response.append("|");
    coord.setNum(type);
    response.append(coord);
    response.append("@");

    qDebug() << "Request Format: Spawn Entity: " << response;
    // Send message to server to Spawn new player
    this->connection->write(response);

    this->initializeToScene();

}
// Constructor for when We catch up
SideScrollEntity::SideScrollEntity(QTcpSocket *connection, QByteArray ID, QGraphicsScene* graphicsScene, QObject* parent) : QObject (parent)
{
    this->connection = connection;
    connect(this->connection, &QTcpSocket::readyRead, this, &SideScrollEntity::recievedData);
    this->ID = ID;
    this->scene = graphicsScene;

    // Use connection to fill in data from server
    this->requestUpdate();

}

void SideScrollEntity::setHP(int val) {
    this->hitpoints = val;
    // Update in server
    QByteArray cmd = "U|HP|";
    QByteArray num;
    num.setNum(val);
    cmd.append(num);
    cmd.append("@");
    this->connection->write(cmd);
    // Finished Writing to server

}
int  SideScrollEntity::getHP() {
    return  this->hitpoints;
}



SideScrollEntity::~SideScrollEntity() {

}


void SideScrollEntity::setLocation(coordinates loc) {
    // epsilon test instead?
    if (loc.x != this->loc.x || loc.y != this->loc.y) {
        this->loc = loc;
        // Update Server
        QByteArray cmd = "U|";
        cmd.append(this->ID);
        cmd.append("|x|");
        QByteArray coord;
        coord.setNum(loc.x);
        cmd.append(coord);
        cmd.append("@");
        cmd.append("U|");
        cmd.append(this->ID);
        cmd.append("|y|");
        coord.setNum(loc.y);
        cmd.append(coord);
        cmd.append("@");
        this->connection->write(cmd);
        // Sent Message to server
        // Other Handling is likely Needed;

        //this->setScreenPos(loc.x, loc.y); // Maybe this is it?
        this->setScreenPos(loc.x, loc.y);
    }
}

void SideScrollEntity::setScreenPos(qreal x, qreal y)
{
    this->loc.x = x;
    this->loc.y = y;
    animation->setScreenPos(x, y);
    // Update Animation
}


void SideScrollEntity::initializeToScene()
{
    if (!this->inScene) {
        animation->setScreenPos(this->loc.x, this->loc.y);
        scene->addItem(animation);
        this->inScene = true;
    }
}


EntityAnimation* SideScrollEntity::getGraphicsItem() const
{
    return this->animation;
}


coordinates SideScrollEntity::getLocation() const
{
    // Automatically updated when recieving a request to update the entity
    // from the server.
    return this->loc;
}

void SideScrollEntity::recievedData() {
    qDebug() << "Recieved Data:";
    QByteArray message = this->connection->readAll();
    if (message.length() > 0) {
        entity_response_buffer = message;
        qDebug() << entity_response_buffer;
    }
    // At this point we have recieved our message, We can update ourself based on
    // Commands inside the message

    int index = 0;
    QByteArray EID;
    QByteArray Value;
    QByteArray State;
    QByteArray X;
    QByteArray Y;
    QByteArray HP;
    QByteArray Type;


    while (index < entity_response_buffer.length()) {

        switch (entity_response_buffer[index]) {

        case 'S' :
            // See Server Modules for Documentation

            if(this->ID == "0") {
                index += 2;

                while (entity_response_buffer[index] != '@') {

                    EID.append(entity_response_buffer[index]);
                    index +=1;
                }

                this->ID = EID;
                qDebug() << "Entity ID of " << this->ID << " Now Assigned";
            }
            // Message not intended for us, Move to @ to start next messge
            while (entity_response_buffer[index] != '@') {
                index +=1;
            }
            break;
        case 'E' :
            index += 2; // skip the first |

            while (entity_response_buffer[index] != '|') {

                EID.append(entity_response_buffer[index]);
                index +=1;
            }

            if (QString(EID) == this->ID) {
                // Message is for us, Update our stats
                index +=1; // Move past |
                while (entity_response_buffer[index] != '|') {
                    X.append(entity_response_buffer[index]);
                    index +=1;
                }
                index +=1; // Move past |
                while (entity_response_buffer[index] != '|') {
                    Y.append(entity_response_buffer[index]);
                    index +=1;
                }
                index +=1; // Move past |
                while (entity_response_buffer[index] != '|') {
                    Type.append(entity_response_buffer[index]);
                    index +=1;
                }
                index +=1; // Move past |
                while (entity_response_buffer[index] != '|') {
                    HP.append(entity_response_buffer[index]);
                    index +=1;
                }
                index +=1; // Move past |
                while (entity_response_buffer[index] != '@') {
                    State.append(entity_response_buffer[index]);
                    index +=1;
                }
                if (debug) {
                    qDebug() << "EID: " << EID;
                    qDebug() << "X: " << X;
                    qDebug() << "Y: " << Y;
                    qDebug() << "Type: " << Type;
                    qDebug() << "HP: " << HP;
                    qDebug() << "State: " << State;
                }

                this->loc.x = X.toDouble();
                this->loc.y = Y.toDouble();
                // Hardcoding a setscreen Local Position,
                // As Side Scrolling is maybe not so crucial to our
                // Game anymore
                // If this is changed,  THis is where that needs to happen
                // -->> ISSUES WITH GLOBAL COORDINATES? HERE IS THE ROOT! <<--
                this->setScreenPos(this->loc.x, this->loc.y);

                // This other stuff is fine tho
                this->type = static_cast<spriteType>( Type.toInt() );
                this->hitpoints = HP.toInt();
                this->animation->setState(static_cast<state>(State.toInt()));

                if (!this->inScene) { // Handles Alternate Construction Method
                    this->initializeToScene();
                }



            } else {
                // Message not intended for us, Move to @ to start next messge
                while (entity_response_buffer[index] != '@') {
                    index +=1;
                }
            }


            break;
        default:
            // Message not intended for us, Move to @ to start next messge
            while (entity_response_buffer[index] != '@') {
                index +=1;
            }
            break;
        }


        index += 1; // Moves past @ sign
    }







}

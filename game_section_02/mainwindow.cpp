#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Allocate and configure scene
    scene = new QGraphicsScene(-SCENE_WIDTH/2, -SCENE_HEIGHT/2, SCENE_WIDTH, SCENE_HEIGHT);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    //QPixmap backgroundImage = QPixmap("loadingscreen.png");
    scene->setBackgroundBrush(Qt::darkCyan);

    // Make scene object the data source for the view object
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    // Ui Connections
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::joinGamePopup);


    // Configure timer object to drive animation
    timer = new QTimer;
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    connect(timer, &QTimer::timeout, this, &MainWindow::MoveEntities);
    timer->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::joinGamePopup()
{

    // Create popup window
    this->popup = new QDialog;                                  // Creates Dialog Popup
    popup->setWindowTitle("Castles and Creatures");             // Set popup Title
    QLabel* IPlabel = new QLabel("Enter IP Address");           // Creates a new Label
    QLabel* Portlabel = new QLabel("Enter Port");               // Creates a new Label
    this->IPlineEdit = new QLineEdit();                         // Creates a new lineEdit
    this->PortlineEdit = new QLineEdit();                       // Creates a new lineEdit
    QPushButton* join = new QPushButton("Join");                // Creates a new pushButton
    QVBoxLayout* layout = new QVBoxLayout(this->popup);         // Creates a Dialog layout

    // Limit Expressions allowed in LineEdits
    // Creates a RegularExpression for IP LineEdit which limits the Input to xxx.xxx.xxx.xxx
    // Where x is an integer between 0 and 9
    QRegularExpression* regexIP = new QRegularExpression("[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}");
    QRegularExpressionValidator* validateIP = new QRegularExpressionValidator(*regexIP);
    // Creates a RegularExpression for Port LineEdit which limits the Input to x
    // Where x is an integer between 0and 9 and the length is between 1 and 65535 digits
    QRegularExpression* regexPort = new QRegularExpression("[0-9]{1,65535}");
    QRegularExpressionValidator* validatePort = new QRegularExpressionValidator(*regexPort);

    // Create the layout for the popup window
    layout->addWidget(IPlabel);                                 // adds IP label to Popup
    layout->addWidget(IPlineEdit);                              // adds IP lineEdit to Popup
    layout->addWidget(Portlabel);                               // adds Port Label to Popup
    layout->addWidget(PortlineEdit);                            // adds Port lineEdit to Popup
    layout->addWidget(join);                                    // adds join button to popup

    // LineEdit validation
    IPlineEdit->setValidator(validateIP);                       // validates lineEdit expression
    PortlineEdit->setValidator(validatePort);                   // validates lineEdit expression

    // connect join button to serverconnect function
    connect(join, &QPushButton::clicked, this, &MainWindow::serverconnect);

    // execute popup
    popup->exec();

}


void MainWindow::serverconnect()
{
    // set IP address to a variable
    this->IP = new QHostAddress(this->IPlineEdit->text());
    qDebug()<< this->IP << '\n';

    // set port to a variable
    bool* isOk = new bool(false);
    this->Port = quint16(this->PortlineEdit->text().toInt(isOk));
    qDebug()<< this->Port << '\n';

    // chack if connection is established
    if(*isOk)
    {

        this->socket = new QTcpSocket(this); // Creates a new TCP socket
        this->socket->setProxy(QNetworkProxy::NoProxy); // sets proxy
        this->socket->connectToHost(*this->IP, this->Port); // Socket connects to Host
        this->socket->waitForConnected(3000); // Socket waits for connection

        if(this->socket->waitForConnected(3000)) // if connection established set connected to true
        {
            this->connected = true; // socket is connected sets variable to true
            qDebug() << "CONNECTED";
            this->popup->close();   // close popup window
            MainWindow::chooseAvatar(); // function call for choose avatar popup
        }
        else
        {
            this->connected = false; // if connection not established set connected to false
            qDebug() << "FAILED TO CONNECT" << socket->errorString();
            delete this->socket;    // delete socket due to no connection
        }

    }

    this->popup->close();

    // Connections
    // connect socket disconnect to a function call to disconnect function
    connect(this->socket, &QTcpSocket::disconnected, this, &MainWindow::serverDisconnected);
}


void MainWindow::serverDisconnected()
{
    this->connected = false; // set conection variable to false due to socket becoming disconnected
    qDebug() << "DISCONNECTED";
}

void MainWindow::chooseAvatar()
{
    // Create Popup Window
    this->avatarPopup = new QDialog;    // creates a new Dialog popup
    avatarPopup->setWindowTitle("Castles and Creatures"); // Set popup Title
    QLabel* avatarlabel = new QLabel("Choose Avatar");  // creates a new label
    avatarlabel->setAlignment(Qt::AlignCenter); // aligns label to center
    QFont font("Arial", 22, QFont::Normal); // sets font, size, and style of font
    avatarlabel->setFont(font); // applies font changes to label
    QPushButton* knightButton = new QPushButton("Knight");  // creates a new button
    QPushButton* alchemistButton = new QPushButton("Alchemist");    // creates a new button
    // QPushButton* mageButton = new QPushButton("Mage");   // creates a new button
    // QPushButton* samuraiButton = new QPushButton("Samurai"); // creates a new button
    QPushButton* slimeButton = new QPushButton("Slime");    // creates a new button
    // QPushButton* skeletonButton = new QPushButton("Skeleton");   // creates a new button

    // Create Dialog window
    QVBoxLayout* window = new QVBoxLayout(this->avatarPopup); // creates a layout for dialog popup

    // Create the layout for the popup window
    window->addWidget(avatarlabel); // adds label to popup
    window->addWidget(knightButton);    // adds button to layout
    window->addWidget(alchemistButton); // adds button to layout
    // window->addWidget(mageButton);   // adds button to layout
    // window->addWidget(samuraiButton);    // adds button to layout
    window->addWidget(slimeButton); // adds button to layout
    // window->addWidget(skeletonButton);   // adds button to layout

    // connects button to function call to set avatarType to knight
    connect(knightButton, &QPushButton::clicked, this, &MainWindow::setKnight);
    // connects button to function call to set avatarType to alchemist
    connect(alchemistButton, &QPushButton::clicked, this, &MainWindow::setalchemist);
    // connects button to function call to set avatarType to mage
    // connect(mageButton, &QPushButton::clicked, this, &MainWindow::setmage);
    // connects button to function call to set avatarType to samurai
    // connect(samuraiButton, &QPushButton::clicked, this, &MainWindow::setsamurai);
    // connects button to function call to set avatarType to slime
    connect(slimeButton, &QPushButton::clicked, this, &MainWindow::setslime);
    // connects button to function call to set avatarType to skeleton
    // connect(skeletonButton, &QPushButton::clicked, this, &MainWindow::setskeleton);

    // execute popup
    avatarPopup->exec();
}

// This function sets the Avater for the player to knight
void MainWindow::setKnight()
{
    avatarType = knight;
    this->avatarPopup->close();
    MainWindow::startGame();
}

// This function sets the Avater for the player to alchemist
void MainWindow::setalchemist()
{
    avatarType = alchemist;
    this->avatarPopup->close();
    MainWindow::startGame();
}

// This function sets the Avater for the player to mage
/* void MainWindow::setmage()
{
    avatarType = mage;
    this->avatarPopup->close();
    MainWindow::startGame();
}

// This function sets the Avater for the player to samurai
 void MainWindow::setsamurai()
{
    avatarType = samurai;
    this->avatarPopup->close();
    MainWindow::startGame();
}
*/

// This function sets the Avater for the player to slime
void MainWindow::setslime()
{
    avatarType = slime;
    this->avatarPopup->close();
    MainWindow::startGame();
}

// This function sets the Avater for the player to skeleton
/* void MainWindow::setskeleton()
{
    avatarType = skeleton;
    this->avatarPopup->close();
    MainWindow::startGame();
}
*/


void MainWindow::startGame()
{
    // Change joinGame Button to closeGameButton
    ui->joinGameButton->setText("Exit Game");
    disconnect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::joinGamePopup);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::close);


    // TODO - Section for Starting RequestEntityRefresh on Timer
    // Additionally needs to connect the socket readyRead to our Response

    // Spawn Player

    this->player = new Player(this->socket, avatarType, scene, 20);
    this->onScreenEntities.append(this->player);


    // During the respond to server, we will check the EIDs of
    // all Entities we have, and see if we are missing any, or have more than we are supposed to.
    // If this is the case, we Use the less verbose Entity constructor to generate the entity
    // Which will load in as soon as it gets all necessary data from the server.
    // Scene manages what entities exist, not what they are doing on server response,
    // we read for the 'R|EID|EID|EID|EID|EID|EID@' Type response. we should then generate
    // Entities with the given EID if we do not have one like that, or, we kill one that
    // does not have a server match ** EXCEPT EID '0' THIS IS STILL AWAITING ASSIGNMENT OF EID **
    // A function that sends an 'R@' message should be periodically called (30 Hz?)
}


void MainWindow::MoveEntities()
{
    if (player != nullptr)
        player->Move(player->GetVelocity());
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e)
    {
        if (player != nullptr)
        {
            if (!e->isAutoRepeat())
            {
                switch (e->key())
                {
                    case Qt::Key_A:
                    case Qt::Key_J:
                    case Qt::Key_Left:
                                            wasd[1] = 1;
                                            player->getGraphicsItem()->setState(Left);
                                            break;

                    case Qt::Key_D:
                    case Qt::Key_L:
                    case Qt::Key_Right:     wasd[3] = 1;
                                            player->getGraphicsItem()->setState(Right);
                                            break;

                    case Qt::Key_W:
                    case Qt::Key_I:
                    case Qt::Key_Up:        wasd[0] = 1;
                                            player->getGraphicsItem()->setState(Up);
                                            break;

                    case Qt::Key_S:
                    case Qt::Key_K:
                    case Qt::Key_Down:      wasd[2] = 1;
                                            player->getGraphicsItem()->setState(Down);
                                            break;

                    case Qt::Key_Space:     //qDebug() << "bang";
                                            break;

                    default:
                                            break;
                }
            }


            player->SetMoveDirection(coordinates{wasd[3]-wasd[1], wasd[0]-wasd[2]});
            qDebug() << "p - " << player->GetVelocity().x << " : " << player->GetVelocity().y;
        }
    }

        QMainWindow::keyPressEvent(e);
}


void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e)
    {
        if (player != nullptr)
        {
            if (!e->isAutoRepeat())
            {
                switch (e->key())
                {
                    case Qt::Key_A:
                    case Qt::Key_J:
                    case Qt::Key_Left:      wasd[1] = 0;
                                            break;

                    case Qt::Key_D:
                    case Qt::Key_L:
                    case Qt::Key_Right:     wasd[3] = 0;
                                            break;

                    case Qt::Key_W:
                    case Qt::Key_I:
                    case Qt::Key_Up:        wasd[0] = 0;
                                            break;

                    case Qt::Key_S:
                    case Qt::Key_K:
                    case Qt::Key_Down:      wasd[2] = 0;
                                            break;

                    case Qt::Key_Space:     //qDebug() << "bang";
                                            break;

                    default:
                                            break;
                }
            }

            player->SetMoveDirection(coordinates{wasd[3]-wasd[1], wasd[0]-wasd[2]});

            if (player->GetVelocity().x == 0.0 && player->GetVelocity().y == 0.0)
                player->getGraphicsItem()->setState(Idle);

            switch ((int)(wasd[0]-wasd[2]))
            {
                case 1:
                    player->getGraphicsItem()->setState(Up); break;
                case -1:
                    player->getGraphicsItem()->setState(Down); break;

                default:
                    break;
            };

            switch ((int)(wasd[3]-wasd[1]))
            {
                case 1:
                    player->getGraphicsItem()->setState(Right); break;
                case -1:
                    player->getGraphicsItem()->setState(Left); break;

                default:
                    break;
            };
        }
    }

    QMainWindow::keyReleaseEvent(e);
}


void MainWindow::requestEntityRefresh() {
    // Server returns In the following form
    // "A|N|N|N|N@"
    //
    this->socket->write("R@"); // Generic Refresh Request;
    if (this->player->getHP() < 0) {
        this->death();
    }
}

// This function should be called when a players HP has reached 0
void MainWindow::death()
{
    // Create Popup Window
    this->deathPopup = new QDialog; // creates a new Dialog popup
    deathPopup->setWindowTitle("Castles and Creatures"); // Set popup Title
    QLabel* deathlabel = new QLabel("YOU HAVE DIED!!!!"); // creates a label
    deathlabel->setAlignment(Qt::AlignCenter);  // aligns label to center
    QFont font("Arial", 42, QFont::Bold);   // sets font, size, style of font
    deathlabel->setFont(font);  // applies font to label
    QLabel* label = new QLabel; // creates a new label
    label->setAlignment(Qt::AlignCenter);   // aligns label to center
    QPixmap* pix = new QPixmap(":/sprites/GameOver.png"); // applies a pixmap to label
    label->setPixmap(pix->scaled(width()/2, height()/2, Qt::KeepAspectRatio));  // sets pixmap size and sets aspect ratio
    QPushButton* exitGameButton = new QPushButton("Exit Game"); // creates a new pushButton


    // Create Dialog window
    QVBoxLayout* deathWindow = new QVBoxLayout(this->deathPopup); // creates a layout for the dialog popup

    // Create the layout for the popup window
    deathWindow->addWidget(deathlabel); // adds label to popup
    deathWindow->addWidget(label);  // adds pixmap to popup
    deathWindow->addWidget(exitGameButton); // adds button to layout

    // Close Game
    // connects pushButton to quit application
    connect(exitGameButton, &QPushButton::clicked, this, &QApplication::quit);

    // execute popup
    deathPopup->exec();
}

void MainWindow::readServerData() {
    qDebug() << "Recieved Data:";
    QByteArray message = this->socket->readAll();
    if (message.length() > 0) {
        entity_response_buffer = message;
        qDebug() << entity_response_buffer;
    }

    int index = 0;
    QByteArray EID;
    QLinkedList<QByteArray> eids;
    bool exists = false;

    while (index < entity_response_buffer.length()) {

        switch (entity_response_buffer[index]) {

        case 'R' :
            // See Server Modules for Documentation


            index += 2; // Move to start of first EID

            while (entity_response_buffer[index] != '@') {

                EID = ""; // Reset EID

                while (entity_response_buffer[index] != '|' &&
                       entity_response_buffer[index] != '@') {

                    EID.append(entity_response_buffer[index]);

                }

                // Append EID to List of EIDs
                eids.append(EID);
                qDebug() << "Recieved Entity : " << EID << ".";

            }

            // Here we need to handle Existing or non-existing Entities

            for (QByteArray ID : eids) {
                // God bless whoever made this iterable
                // for QLinkedLists
                exists = false; // Reset Exists Variable
                for (SideScrollEntity* entity : this->onScreenEntities) {
                    if (entity->ID == ID) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    // Create Matching Entity
                    this->onScreenEntities.append(new
                    SideScrollEntity(this->socket, ID, this->scene));
                }
            }
            // TODO - KILL  NON-Existing- Entities!

            for (SideScrollEntity * entity: this->onScreenEntities) {
                exists = false;
                for (QByteArray ID: eids) {
                    if (entity->ID == ID) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    this->onScreenEntities.removeOne(entity);
                    delete entity;
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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QtDebug>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkProxy>
#include <QLinkedList>
#include "player.h"
#include "sidescrollentity.h"
#include "defs.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>


#include <QtMath>

#include "player.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Variables
    QHostAddress* IP;
    quint16 Port;

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:
    Ui::MainWindow *ui;

    QTimer* timer;

    QDialog* popup;
    QDialog* avatarPopup;
    QDialog* deathPopup;
    QLineEdit* IPlineEdit;
    QLineEdit* PortlineEdit;
    QGraphicsScene* scene = nullptr;
    spriteType avatarType = knight;

    QTcpSocket* socket;
    bool connected = false;

    Player* player = nullptr;
    qreal wasd[4] = {0, 0, 0, 0};
    void requestEntityRefresh();

    QLinkedList<SideScrollEntity*> onScreenEntities;


private slots:
    void joinGamePopup();
    void serverconnect();
    void serverDisconnected();
    void chooseAvatar();
    void setKnight();
    void setalchemist();
    // void setmage();
    // void setsamurai();
    void setslime();
    // void setskeleton();

    void startGame();
    void death();


    void readServerData();

    void MoveEntities();


};


#endif // MAINWINDOW_H

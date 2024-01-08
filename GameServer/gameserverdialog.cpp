#include "gameserverdialog.h"
#include "ui_gameserverdialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent),
                                  ui(new Ui::Dialog)
{
    ui->setupUi(this);
    tcpServer = new MyTCPServer(this);
    if (!tcpServer->listen())
    {
        qDebug() << "Unable to start the server: " << tcpServer->errorString();
        close();
        return;
    }
    else
    {
        qDebug() << "Server started!\n";
    }

    QString localhostname = QHostInfo::localHostName();
    QString localhostIP;
    QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    foreach (const QHostAddress &address, hostList)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false)
        {
            localhostIP = address.toString();
        }
    }

    QString localMacAddress;
    QString localNetmask;
    foreach (const QNetworkInterface &networkInterface, QNetworkInterface::allInterfaces())
    {
        foreach (const QNetworkAddressEntry &entry, networkInterface.addressEntries())
        {
            if (entry.ip().toString() == localhostIP)
            {
                localMacAddress = networkInterface.hardwareAddress();
                localNetmask = entry.netmask().toString();
                break;
            }
        }
    }

    QString localNetmaskCIDR = QString(" /%1").arg(netmaskToCIDR(localNetmask));

    qDebug() << "Retrieving server information...\n";
    qDebug() << "Localhost name: " << localhostname;
    qDebug() << "IP = " << localhostIP;
    qDebug() << "Netmask = " << localNetmask;
    qDebug() << "Netmask CIDR =" << localNetmaskCIDR;
    qDebug() << "MAC = " << localMacAddress;

    ui->portNumberLabel->setNum(tcpServer->serverPort());
    ui->ipAddressLabel->setText(localhostIP);
    ui->localAddressLabel->setText("127.0.0.1");

    connect(tcpServer, &MyTCPServer::forwardData, this, &Dialog::incomingConnection);

    initDatabase();
}

Dialog::~Dialog()
{
    delete ui;
    delete tcpServer;
}
// Receiving error message on Client DC:
// "QSocketNotifier: Invalid socket 11 and type 'Read', disabling..."
void Dialog::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "Incoming connection...";
    QTcpSocket *client = new QTcpSocket(this);
    if (!client->setSocketDescriptor(socketDescriptor))
    {
        // Verify new connection is actually a client
        qDebug() << "Error: Invalid socket descriptor!";
        delete client;
        return;
    }
    client->setProxy(QNetworkProxy::NoProxy);
    clients.insert(client);
    ui->textBrowser->append(QString("NEW CONNECTION"));
    connect(client, &QTcpSocket::readyRead, this, &Dialog::readClient);
    // Changed client from capture by reference to value; lamda function handled referece incorrectly
    // when attempting to remove DC'd client from tableView and DB
    connect(client, &QTcpSocket::disconnected, [this, client = client]() // CHECKME original: [this, client]()
            {
        clients.remove(client);
        QString entityID = clientEntityMap.value(client); // CHECKME
        clientEntityMap.remove(client); // CHECKME

        // Find the row that corresponds to the disconnected client
        int rowToRemove = -1;
        for (int row = 0; row < qm.rowCount(); row++)
        {
            if (qm.record(row).value("EID").toString() == entityID)
            {
                rowToRemove = row;
                break;
            }
        }
        // Remove the row from the model
        if (rowToRemove != -1)
        {
            QSqlQuery action;
            action.exec(QString("DELETE FROM entities WHERE EID = %1").arg(entityID));
            refreshTable();
        }
        client->deleteLater();
        ui->textBrowser->append(QString("CLIENT DISCONNECTED, EID: %1").arg(entityID));
    });
}
void Dialog::readClient()
{
    QTcpSocket *client = static_cast<QTcpSocket *>(sender());

    static int nextEID = 1;

    // read data from the client
    QByteArray ba = client->readAll();
    QString s(ba);
    ui->textBrowser->append(s);

    // respond to the request
    // first char will determine type of request made
    int index = 0;
    int lIndex = 0;
    QByteArray EID;
    QByteArray Attribute;
    QByteArray Value;
    QByteArray State;
    QByteArray X;
    QByteArray Y;
    QByteArray Type;
    QSqlQuery action; // Defaults to open connection
    QString sqlStatement;
    QByteArray retVal;

    while (index < ba.length()) {


    switch (ba[index]) {

    case 'R':
        // PASS
        // Client is asking for refresh of data
        // 'R@'
        // In future, may send coordinates and a radius to return only entities in a radius.
        // Server returns In the following form
        // "R|N|N|N|N@"
        // | Indicates seperator between slots
        // @ indicates end of message
        // Should contain a newline to seperate from the header and each entity
        // Spaces are used for readability, but not present in actual message
        //
        // A Indicates Acknowledgement
        index +=1; // Move to next command
        sqlStatement = QString("SELECT * FROM entities");
        if (!action.exec(sqlStatement)) {
            qDebug() << "Error executing statement";
        }

        retVal = "R";
        while (action.next()) {
            QString temp = "|" + action.value(0).toString();
            retVal.append(temp);
        }
        retVal.append("@");

        qDebug() << "Recieved request for EID List";
        qDebug() << "Response : " << retVal;


        client->write(retVal);


        break;
    case 'E':
        //
        // Client is asking for refresh of data for a specific entity
        // Request format
        // "E|EID@"
        // Server returns In the following form
        // "E|EID|X|Y|TYPE|HP|STATE@"
        // | Indicates seperator between slots
        // @ indicates end of message
        // Should contain a newline to seperate from the header and each entity
        // Spaces are used for readability, but not present in actual message
        //
        // A Indicates acknowledge

        // Test String
        // E|1@

        index +=2;
        lIndex = 0;
        while (ba[index] != '@') {
            EID[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // For moving to next command

        // Obtained EID

        sqlStatement = QString("SELECT * FROM entities WHERE EID=%1").arg(QString(EID));
        if (!action.exec(sqlStatement)) {
            qDebug() << "Error executing statement";
        }

        retVal = "E";
        while (action.next()) {
            QString temp = "|" + action.value(0).toString() +
                    "|" + action.value(1).toString() +
                    "|" + action.value(2).toString() +
                    "|" + action.value(3).toString()+
                    "|" + action.value(4).toString()+
                    "|" + action.value(5).toString();
            retVal.append(temp);
        }
        retVal.append("@");

        if (retVal.length() < 5) {
            retVal = "E|DEAD@";
        }

        qDebug() << "Recieved request for Entity Data";
        qDebug() << "Response : " << retVal;



        client->write(retVal);



        break;
    case 'S':
        // PASS (Correctly Works)
        // Client wants to spawn new entity
        // Client will send the entity type, as well as the start point for the entity.
        // should send back EID.
        // Client send format "S|STATE|X|Y|TYPE@"
        // | Indicates seperator between slots
        // Server returns "S|EID@"
        // A Is for approval
        // @ indicates end of message

        // Test String for NC
        // S|0|121.3|169.2|4@
        index += 2;

        while (ba[index] != '|') {
            State[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // Move Past |
        lIndex = 0;
        while (ba[index] != '|') {
            X[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // Move Past |
        lIndex = 0;
        while (ba[index] != '|') {
            Y[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // Move Past |
        lIndex = 0;
        while (ba[index] != '@') {
            Type[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // For moving to next command



        qDebug() << "Recieved Request to spawn new entity";
        qDebug() << "UNPACKED: ";
        qDebug() << "State: " <<  State;
        qDebug() << "X: " << X;
        qDebug() << "Y: " << Y;
        qDebug() << "Type: " << Type;


        qDebug() << "Respond with: ";
        qDebug() << "Assigned EID: " << nextEID;

        sqlStatement = QString("INSERT INTO entities (EID, x, y, type, HP, state) VALUES (%1, %2, %3, %4, %5, %6);")
                .arg(nextEID).arg(QString(X)).arg(QString(Y)).arg(QString(Type)).arg(100).arg(QString(State));
        if (!action.exec(sqlStatement)) {
            qDebug() << "Error executing statement";
        }


        qDebug() << "Executed Statement: " << sqlStatement;
        clientEntityMap.insert(client, QString::number(nextEID));
        retVal.setNum(nextEID);
        retVal.prepend("S|");
        retVal.append("@");
        client->write(retVal);
        nextEID +=1;
        this->refreshTable();
        break;
    case 'U':
        // PASS
        // Client wants to update information of an existing entity
        // client should send an EID, an attribute, and a value to update
        // Client's message should look like:
        // "U|EID|ATRRIBUTE|VALUE@"
        // | Indicates seperator between slots
        // @ indicates end of message
        // Server does not have to return anything.

        // Test String
        // U|1|HP|1000000@


        index += 2;

        while (ba[index] != '|') {
            EID[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // Move Past |
        lIndex = 0;
        while (ba[index] != '|') {
            Attribute[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }


        index +=1; // Move Past |
        lIndex = 0;
        while (ba[index] != '@') {
            Value[lIndex] = ba[index];
            index +=1;
            lIndex +=1;
        }

        index +=1; // For moving to next command

        qDebug() << "Received Request to change Data of existing entity";
        qDebug() << "UNPACKED: ";
        qDebug() << "EID: " <<  EID;
        qDebug() << "ATTRIBUTE: " << Attribute;
        qDebug() << "VALUE: " << Value;

        sqlStatement = QString("UPDATE entities SET %1=%2 WHERE EID=%3")
                .arg(QString(Attribute)).arg(QString(Value)).arg(QString(EID));
        if (!action.exec(sqlStatement)) {
            qDebug() << "Error executing statement";
        }


        qDebug() << "Executed Statement: " << sqlStatement;

        this->refreshTable();
        break;
    default:
        qDebug() << "Unrecognized Request";
        qDebug() << ba;
        return;

    }
    }


}
int Dialog::netmaskToCIDR(QString netmask)
{
    QStringList list = netmask.split(".");
    QString binaryStr;

    foreach (QString item, list)
    {
        binaryStr += QString("%1").arg(item.toInt(), 8, 2, QChar('0'));
    }

    return binaryStr.count("1");
}
void Dialog::initDatabase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("GameServerDatabase.db");
    if (!this->db.open())
    {
        qDebug() << "\nError: Failed to connect to database.";
        exit(1);
    }
    else
    {
        qDebug() << "\nDatabase connection established.";
    }

    QSqlQuery qprep; // Defaults to only DB open.

    qprep.exec("DROP TABLE entities");
    if ( !qprep.exec("CREATE TABLE IF NOT EXISTS entities (\
                     EID INTEGER PRIMARY KEY,\
                     x DOUBLE,\
                     y DOUBLE,\
                     type INTEGER,\
                     HP INTEGER,\
                     state INTEGER\
                     \
                 );") ) {
        qDebug() << "Error Creating table: " << db.lastError();
        return;
    }

    qprep.exec("SELECT * FROM entities");

    this->qm.setQuery(qprep);
    ui->tableView->setModel(&this->qm);
}

void Dialog::refreshTable() {
         QSqlQuery qprep; // Defaults to only DB open.
         qprep.exec("SELECT * FROM entities");
         this->qm.setQuery(qprep);
}

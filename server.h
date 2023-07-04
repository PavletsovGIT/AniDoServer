#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QDir>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();
    QTcpSocket *socket;
    QByteArray Data;

    QJsonDocument doc;
    QJsonParseError docError;
    QDir fileDir;
    QDir filePath;
    QString pathToFile;
    QString fileName;
    QProcess splitter;
    QString command;
    QString templateName;
    QString samplesPath = "C:\\Qt\\Qt_Apps\\server_dir\\samples\\";
    QString vName;

public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisconnect();
};

#endif // SERVER_H

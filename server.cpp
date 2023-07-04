#include "server.h"

Server::Server() {}
Server::~Server() {}

void Server::startServer() {
    if (this->listen(QHostAddress::Any, 9679)) {
    } else {
        qDebug() << "Error: Can't start server";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisctonnect()));

    qDebug() << socketDescriptor << "Client connected";

    //"type":"connect","status":"yes"
    socket->write("{\"type\":\"connect\",\"status\":\"yes\"}");
    qDebug() << "Send client connect status - YES";
}

void Server::sockReady() {
    Data = socket->readAll();
    qDebug() << "Data: " << Data;

    doc = QJsonDocument::fromJson(Data, &docError);

    //{"type":"select", "params":"workers"}
    //docError.errorString() == "no error occurred"
    if (docError.errorString() == "no error occurred")
    {
        if (doc.object().value("type").toString() == "Find_File")
        {
            fileName = doc.object().value("message").toString();
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            fileDir = samplesPath + fileName;

            if (fileDir.exists())
            {
                QByteArray resultToSend = "{\"type\":\"Find_Result\",\"message\":\"File_Found\"}";

                socket->write(resultToSend);
                socket->waitForBytesWritten(2000);
            }
            else
            {
                QByteArray resultToSend = "{\"type\":\"Find_Result\",\"message\":\"File_Not_Found\"}";

                socket->write(resultToSend);
                socket->waitForBytesWritten(2000);
            }
        }
        //
        else if (doc.object().value("type").toString() == "Download")
        {
            fileName = doc.object().value("message").toString();
            QFile file("C:\\Qt\\Qt_Apps\\server_dir\\samples\\" + fileName);
            QString servSecDir = "C:\\Qt\\Qt_Apps\\client_dir\\samples\\";
            file.copy(servSecDir);
        }
    }
}

void Server::sockDisconnect() {
    qDebug() << "Disconnect";
    socket->deleteLater();
}

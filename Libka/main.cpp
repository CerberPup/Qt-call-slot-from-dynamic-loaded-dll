#include "main.h"

#include <iostream>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>


namespace parse
{
    QString list_widgets()
    {
        QString lista;

        const QWidgetList allWidgets = QApplication::allWidgets();
        for (QWidget *widget : allWidgets)
        {
            lista += widget->objectName();
            if (widget != allWidgets.last())
            {
                lista += ":";
            }
        }
        return lista;
    }

    QString call_slot(QStringList& _name, QString& _slot)
    {
        const QWidgetList allWidgets = QApplication::allWidgets();
        for (QWidget *widget : allWidgets)
        {
            if (_name.first() == widget->objectName())
            {
                if (_name.size() != 1)
                {
                    _name.pop_front();
                    continue;
                }
                if (QMetaObject::invokeMethod( widget, _slot.toStdString().c_str()/*, Q_ARG( int, param ) */))
                {
                    return "Called from widget: \"" + _name.first() + "\" slot: \"" + _slot + '"';
                }
                else
                {
                    return "Failed to call from widget: \"" + _name.first() + "\" slot: \"" + _slot + '"';
                }
                
            }
        }
        return "Failed to find \"" + _name.first() + '"';
    }

    QString parse(QString _str)
    {
        QStringList lista = _str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (lista.empty())
        {
            return "Empty message";
        }

        if (lista.first() == "list")
        {
            return list_widgets();
        }
        if (lista.first() == "call")
        {
            lista.pop_front();
            if (lista.size() < 2)
            {
                return "Missing widget name or slot name";
            }
            
            QString slot = lista.last();
            lista.pop_back();
            return call_slot(lista, slot);
        }
        return "Failed to parse command";
    }
}

listener::listener(QObject *parent):QObject(parent), _server(this)
{
    _server.listen(QHostAddress::Any, 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void listener::onNewConnection()
{
   QTcpSocket *clientSocket = _server.nextPendingConnection();
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
   connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    for (QTcpSocket* socket : _sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
        std::cout << clientSocket->peerAddress().toString().toStdString() + " connected to server !\n" << std::endl;
    }
}

void listener::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void listener::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();
    for (QTcpSocket* socket : _sockets) {
        if (socket != sender)
            socket->write(0);
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
            std::cout << sender->peerAddress().toString().toStdString() + ": " + datas.toStdString() << std::endl;
            std::string parsed = parse::parse(datas).toStdString();
            std::cout << parsed;
            socket->write(QByteArray::fromStdString(parsed) + "\n");
    }
}

static listener* listener_;

__attribute__((constructor)) void init(void) 
{
    std::cout << "Pizdryk init" << std::endl;
    listener_ = new listener();
};

__attribute__((destructor))  void fini(void) 
{
    std::cout << "Pizdryk fini" << std::endl;
    delete listener_;
};
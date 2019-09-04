#include <string>

#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class listener: public QObject
{
    Q_OBJECT

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
private:
    QTcpServer  _server;
    QList<QTcpSocket*>  _sockets;
public:
    listener(QObject *parent = nullptr);
};
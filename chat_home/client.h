#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtEndian>

//json 头文件
#include <json.h>
#include <value.h>
#include <reader.h>
enum ClientSig{
    //socket连接
    connect,
    disconnect,
    error,

    //用户列表
    init_users,
    user_add,
    user_del,

    //接收数据
    recv_msg
};
class Client : public QObject
{
    Q_OBJECT
public:

    Client(QString addr, short port, QObject *parent = nullptr);
    ~Client();
    void sendMsg(short id, QString msg);
    void changeConnection(QString newHost, short newPort);

signals:
    void recvMsg_sig(QString recvMsg);

    void client_sig(ClientSig sig, QString Msg);

private:
    QTcpSocket sock;

private slots:


};

#endif // CLIENT_H

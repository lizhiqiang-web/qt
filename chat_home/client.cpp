#include "client.h"

#define MAX_LENGTH 1024*2


Client::Client(QString addr, short port,QObject *parent) : QObject(parent)
{
    sock.connectToHost(addr, port);

    connect(&sock, &QTcpSocket::connected, this, [=](){
        qDebug() << "connect successfully:";

        emit client_sig(ClientSig::connect, nullptr);
    });

    connect(&sock, &QTcpSocket::disconnected, this, [=](){

        qDebug() << "disconnect";
        emit client_sig(ClientSig::disconnect,"disconnect");
    });

    connect(&sock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        [=](QAbstractSocket::SocketError sock_error){
         qDebug() << "socke error:" << sock.errorString();

         emit client_sig(ClientSig::error,sock.errorString());
    });
    //异步接收信息
    connect(&sock, &QTcpSocket::readyRead, this,[=](){
        QByteArray msg = sock.readAll();
        unsigned short msgid = 0;
        unsigned short msg_len = 0;

        memcpy(&msgid, msg.data(),2);
        memcpy(&msg_len, msg.data()+2,2);

        msgid = qToLittleEndian(msgid);

        msg_len = qToLittleEndian(msg_len);
        qDebug() << msg_len;
        qDebug() << msgid;
        QJsonDocument doc = QJsonDocument::fromJson(msg.data()+4);
        qDebug() <<  doc;
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();



             if( obj["id"].toInt() == 997)
             {
                 //刚加入要初始化用户列表
                 //自己加入
                 qDebug() << "names :" <<  obj["names"].toString();
                 emit client_sig(init_users, obj["names"].toString());
                 emit client_sig(recv_msg, obj["data"].toString());


             }
             else if(obj["id"].toInt() == 998)
             {
                 //添加对应用户列
                 //消息某某加入
                 emit client_sig(user_add, obj["add_name"].toString());
                 emit client_sig(recv_msg, obj["data"].toString());
             }
             else if(obj["id"].toInt() == 999)
             {
                 //删除用户列表的对应元素
                 //消息某某推出
                 emit client_sig(user_del, obj["del_name"].toString());
                 emit client_sig(recv_msg, obj["data"].toString());
             }
               else if(obj["id"].toInt() == 1000)
             {
                emit client_sig(recv_msg, obj["data"].toString());
             }
         }
    });


}

Client::~Client()
{
     sock.close();
     sock.deleteLater();
}
void Client::sendMsg(short id, QString msg)
{
    /*Json::Value root;
    root["id"] = id;
    root["data"] = msg.toStdString().c_str();
    std::string request = root.toStyledString();
    unsigned short request_length = request.length();
    char send_data[MAX_LENGTH] = {0};
    int msgid_host = qToBigEndian(id);
    memcpy(send_data, &msgid_host, 2);
    // 转为网络字节序
    int request_host_length = qToBigEndian(request_length);
    memcpy(send_data + 2, &request_host_length, 2);
    memcpy(send_data + 4, request.c_str(), request_length);
    qDebug() << QString::fromStdString(request);
    qDebug() << "bigen write";
   ;*/
    char send_data[MAX_LENGTH] = {0};
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("data",msg.toStdString().c_str());
    QJsonDocument doc(obj);
    QByteArray request = doc.toJson();
    int msgid_host = qToBigEndian(id);
    memcpy(send_data, &msgid_host, 2);

    unsigned short request_length = request.length();
    unsigned short request_host_length = qToBigEndian(request_length);
    memcpy(send_data + 2, &request_host_length, 2);
    memcpy(send_data + 4, request.toStdString().c_str(), request_length);

     sock.write(send_data, request_length + 4);
    if(!sock.waitForBytesWritten())
    {
        qDebug() << "fail to send";
    }
}

void Client::changeConnection(QString newHost, short newPort) {
    // 关闭旧的连接
    if (sock.state() == QAbstractSocket::ConnectedState) {
        sock.disconnectFromHost();
        sock.waitForDisconnected();
    }
    // 连接到新的IP地址和端口
    sock.connectToHost(QHostAddress(newHost), newPort);

}


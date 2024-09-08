#ifndef CHATWDT_H
#define CHATWDT_H

#include <QWidget>
#include <QObject>
#include <QtNetwork>>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include "client.h"
#include <QStringListModel>
#include <QModelIndex>
namespace Ui {
class ChatWdt;
}

class ChatWdt : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWdt( Client &client,QWidget *parent = nullptr);
    ~ChatWdt();
public slots:


private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatWdt *ui;
    Client &_client;
};

#endif // CHATWDT_H

#include "chatwdt.h"
#include "ui_chatwdt.h"

ChatWdt::ChatWdt( Client &client,QWidget *parent) :   _client(client),
    QWidget(parent),
    ui(new Ui::ChatWdt)
{
    ui->setupUi(this);

    static QStringListModel *model = new QStringListModel(this);
    ui->recv_txt->setReadOnly(true);
    connect(&_client,&Client::client_sig, this,[&](ClientSig sig, QString msg){
        if(sig == ClientSig::disconnect || sig == recv_msg)
        {
            ui->recv_txt->append(msg + "\n");
        }
        else if(sig == init_users)
        {
            qDebug() << msg;
            QStringList names(msg.split(","));
            model->setStringList(names);
            ui->user_list->setModel(model);
            //显示用户列表
        }
        else if(sig == user_add)
        {
            model->insertRow(model->rowCount());
            QModelIndex index = model->index(model->rowCount()-1);
            model->setData(index,msg,Qt::DisplayRole);
        }
        else if(sig == user_del)
        {
            //
            QModelIndexList matches = model->match(model->index(0, 0),
                                                 Qt::DisplayRole,
                                                 msg,
                                                 1,
                                                 Qt::MatchRecursive);
            for (auto it : matches) {
                model->removeRow(it.row());
            }
        }

    });

}

ChatWdt::~ChatWdt()
{
    delete ui;
}




void ChatWdt::on_send_btn_clicked()
{
    _client.sendMsg(1000,ui->send_txt->toPlainText());
}


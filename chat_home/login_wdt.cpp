#include "login_wdt.h"
#include "ui_login_wdt.h"
#include "chatwdt.h"
enum LoginError{

};

LoginWdt::LoginWdt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

}

LoginWdt::~LoginWdt()
{
    delete ui;
}


void LoginWdt::on_login_btn_clicked()
{
    ui->error_lab->setText("请稍等...");
    static Client *client = nullptr;
    if(client == nullptr)
    {
        client = new Client(ui->ip_ledt->text(),ui->port_ledt->text().toInt(), this);
    }
    else
    {
        client->changeConnection(ui->ip_ledt->text(),ui->port_ledt->text().toInt());
    }
    connect(client,&Client::client_sig, this,[&](ClientSig sig,QString str_error){
        if(sig == ClientSig::connect)
        {
            client->sendMsg(1001, ui->name_ledt->text().toUtf8());
            ChatWdt *chat_wdt = new ChatWdt(*client);
            close();

            chat_wdt->show();

        }
        else if(sig == ClientSig::error)
        {
            ui->error_lab->setText(str_error);
        }
    });

}

void LoginWdt::on_quit_btn_clicked()
{
    close();
}

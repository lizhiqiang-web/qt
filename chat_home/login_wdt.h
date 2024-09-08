#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <memory>
#include "client.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE




class LoginWdt : public QWidget
{
    Q_OBJECT

public:
    LoginWdt(QWidget *parent = nullptr);
    ~LoginWdt();

private slots:
    void on_login_btn_clicked();

    void on_quit_btn_clicked();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H

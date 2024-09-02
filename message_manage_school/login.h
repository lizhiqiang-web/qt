#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <student.h>
//#include "publicinfo.h"
QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QWidget
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();

protected:
//    virtual void mousePressEvent(QMouseEvent *event) override;
//    virtual void mouseMoveEvent(QMouseEvent *event) override;
//    QPoint windowPos;
//    QPoint mousePos;
//    QPoint dPos;
private slots:


    void on_login_btn_clicked();

private:
    Ui::login *ui;
    QSqlDatabase db;

};
#endif // LOGIN_H

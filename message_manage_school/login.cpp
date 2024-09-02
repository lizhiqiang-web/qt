#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include "managewidget.h"
#include "studentwidget.h"
#include <QString>
//using namespace  sqldb;

const QString  DATABASE_PATH = "D:\\64softs\\sqlite\\databases\\school.db";
const QString  QDRIVER_TYPE = "QSQLITE";
login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    setFixedSize(540, 415);
    ui->stu_rbtn->click();
    initDataBase(this->db, this);

}

login::~login()
{
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    delete ui;
}


void login::on_login_btn_clicked()
{
    enum UserType{

        student,
        teacher,
        worker,
        manager

    }user_type;

    QString unm = ui->unm_ledt->text();
    QString pwd = ui->pwd_ledt->text();
    QSqlQuery query;
    QString sql;

    if(ui->stu_rbtn->isChecked())
    {
        user_type = student;
        qDebug("student");
        sql = "select * from login_student where  id = " + unm + ";";
    }
    else if (ui->teach_rbtn->isChecked())
    {
        user_type = teacher;
        qDebug("teacher");
        sql = "select * from student_login where username = " + unm + ";";
    }
    else if (ui->work_rbtn->isChecked())
    {
        user_type = worker;
        qDebug("worker");
        sql = "select * from student_login where username = " + unm + ";";
    }
    else if (ui->manager_rbtn->isChecked())
    {
        user_type = manager;
        qDebug("manage");
        sql = "select * from login_manager where id = " + unm + ";";
    }
    if(query.exec(sql))
    {

          if (query.next())
          {
                qDebug() <<"login_id" << query.value("id").toString();
                qDebug() <<"login_id" << query.value("pwd").toString();

                if(pwd == query.value("pwd").toString())
                {
                     //this->hide();
                     qDebug() <<"login_sucess";
                      QWidget *wdt = NULL;
                     switch (user_type)
                     {
                          case student:
                               wdt = new StudentWidget(db,unm.toInt());

                          break;
                          case teacher:
                                wdt = NULL;
                          break;
                          case worker:
                                wdt = NULL;
                          break;
                          case manager:
                                wdt = new ManageWidget(db);
                          break;

                     }
                     if(wdt != NULL)
                     {
                        this->close();
                        wdt->show();
                     }
                  }
                  else
                  {
                      QMessageBox::information(this, "提示信息", "密码错误请重试");
                  }

           }
           else
           {
            QMessageBox::information(this, "提示信息", "用户名错误或者不存在");
           }
    }
    else
    {
        QSqlError sqlError = query.lastError();

        if(sqlError.type() == QSqlError::UnknownError)
        {
            qDebug() << "unkonw error";
            QMessageBox::information(this, "提示信息", "未知错误");
        }
        else if(sqlError.type() == QSqlError::StatementError)
        {
            qDebug() << "lag error";
        }

    }
}

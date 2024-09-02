#include "studentwidget.h"
#include "ui_studentwidget.h"

const QString  DATABASE_PATH = "D:\\64softs\\sqlite\\databases\\school.db";
const QString  QDRIVER_TYPE = "QSQLITE";
StudentWidget::StudentWidget(QSqlDatabase &database,int id, QWidget *parent) :
    QWidget(parent), db(database),
    ui(new Ui::StudentWidget)
{
    ui->setupUi(this);
    setFixedSize(680,550);

    setLineEidtsORead(true);

    db = QSqlDatabase::addDatabase(QDRIVER_TYPE);
    db.setDatabaseName(DATABASE_PATH);
//      mysql远程网络使用
//    db.setHostName("localhost");
//    db.setPort(3306);
//    db.setUserName("root");
//    db.setPassword("123456");

    if(db.open())
    {
        qDebug() << "success to open database";
    }
    else
    {
        QSqlError sqlError =  db.lastError() ;
        QMessageBox *msg_bx = new QMessageBox(this);
        msg_bx->setWindowTitle("信息提示");
        msg_bx->setDefaultButton(QMessageBox::Ok);
        if(sqlError.type() == QSqlError::ConnectionError)
        {

            msg_bx->setText("连接失败");

        }
        else if (sqlError.type() == QSqlError::UnknownError)
        {
            msg_bx->setText("未知错误请重新运行");
        }
         msg_bx->show();
    }
    QString sql = "select * from student_msg where id= " + QString::number(id) + ";";
    showSqlSelectedIitem(sql);
}

StudentWidget::~StudentWidget()
{
    //db.close();
    delete ui;
}
void StudentWidget::setLineEidtsORead(bool en)
{   if(en == true)
    {
        ui->lineEdit_1->setReadOnly(true);
        ui->lineEdit_2->setReadOnly(true);
        ui->lineEdit_3->setReadOnly(true);
        ui->lineEdit_4->setReadOnly(true);
        ui->lineEdit_5->setReadOnly(true);
        ui->lineEdit_6->setReadOnly(true);
        ui->lineEdit_7->setReadOnly(true);
        ui->lineEdit_8->setReadOnly(true);
    }
    else
    {
        ui->lineEdit_1->setReadOnly(false);
        ui->lineEdit_2->setReadOnly(false);
        ui->lineEdit_3->setReadOnly(false);
        ui->lineEdit_4->setReadOnly(false);
        ui->lineEdit_5->setReadOnly(false);
        ui->lineEdit_6->setReadOnly(false);
        ui->lineEdit_7->setReadOnly(false);
        ui->lineEdit_8->setReadOnly(false);
    }

}

void StudentWidget::showSqlSelectedIitem(QString sql)
{

   QSqlQuery query;
   if(query.exec(sql))
   {
       while(query.next())
       {
            ui->lineEdit_1->setText(query.value(0).toString());
            ui->lineEdit_2->setText(query.value(1).toString());
            ui->lineEdit_3->setText(query.value(2).toString());
            ui->lineEdit_4->setText(query.value(3).toString());
            ui->lineEdit_5->setText(query.value(4).toString());
            ui->lineEdit_6->setText(query.value(5).toString());
            ui->lineEdit_7->setText(query.value(6).toString());
            ui->lineEdit_8->setText(query.value(7).toString());
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


void StudentWidget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    QString text = item->text();
    if(text == "个人信息")
    {
        ui->stackedWidget->setCurrentWidget(ui->msg);
    }
    else if(text == "成绩")
    {
        ui->stackedWidget->setCurrentWidget(ui->score);
    }
}



void StudentWidget::on_updata_btn_clicked()
{
    static QStringList list;
    if(ui->updata_btn->text() == "修改")
    {
         list.clear();
         list << ui->lineEdit_1->text()
                << ui->lineEdit_2->text()
                 << ui->lineEdit_3->text()
                    << ui->lineEdit_4->text()
                       << ui->lineEdit_5->text()
                          << ui->lineEdit_6->text()
                             << ui->lineEdit_7->text()
                                << ui->lineEdit_8->text();

         setLineEidtsORead(false);
         ui->updata_btn->setText("保存/取消");
     }
     else
    {
            setLineEidtsORead(true);
            ui->updata_btn->setText("修改");
         int ret = QMessageBox::question(this, "提示", "是否保存",QMessageBox::Save | QMessageBox::Cancel);
         if(ret == QMessageBox::Save)
         {
             //数据库操作
             QString id = list.takeFirst();
             QString sql = "delete from student_msg where id = " + id + ";";

             bool ret = exeChangeDataBaseSql(sql,this->db);
             if(ret == true)
             {
                 sql = "insert into student_msg values(" + id + ",\'" + list.join("\',\'")  + "\', \'\',\'\',\'\',\'\',\'\');";
                 ret = exeChangeDataBaseSql(sql,this->db);
                 if(ret == true)
                 {
                     return;
                 }
             }
         }
         ui->lineEdit_1->setText(list.at(0));
         ui->lineEdit_2->setText(list.at(1));
         ui->lineEdit_3->setText(list.at(2));
         ui->lineEdit_4->setText(list.at(3));
         ui->lineEdit_5->setText(list.at(4));
         ui->lineEdit_6->setText(list.at(5));
         ui->lineEdit_7->setText(list.at(6));
         ui->lineEdit_8->setText(list.at(7));
    }


}

#include "publicinfo.h"
static const QString  DATABASE_PATH = "D:\\64softs\\sqlite\\databases\\school.db";
static const QString  QDRIVER_TYPE = "QSQLITE";
void initDataBase(QSqlDatabase &db, QWidget *parent)
{
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
        QMessageBox *msg_bx = new QMessageBox(parent);
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

}
bool exeChangeDataBaseSql(QString sql,QSqlDatabase &db,QWidget *parent)
{
      QSqlQuery query;
      db.transaction();
      if(query.exec(sql))
       {
          db.commit();
          return true;
       }
       else
       {
         db.rollback();
         QSqlError sqlError = query.lastError();

          if(sqlError.type() == QSqlError::UnknownError)
         {
             qDebug() << "unkonw error";
             QMessageBox::information(parent, "提示信息", "未知错误");
         }
         else if(sqlError.type() == QSqlError::StatementError)
         {
             qDebug() << "lag error";
         }

       }
      return false;
}

void showSqlSelectedIitem(QString sql,QWidget *parent,doAfterQueryExe)
{
   //tablewiget->clearContents();
//   tablewiget->setRowCount(0);
   QSqlQuery query;
   if(query.exec(sql))
   {
       qDebug()<<query.size();
       int row = 0;

       while(query.next())
       {

//           tablewiget->setRowCount(row + 1);
//           for (auto i =0 ; i < tablewiget->columnCount(); i++)
//           {
//               QTableWidgetItem *tabwdt_itm = new QTableWidgetItem();
//               Qt::ItemFlags falgs = tabwdt_itm->flags() & ~Qt::ItemIsEditable;
//              tabwdt_itm->setFlags(falgs);
//               tabwdt_itm->setText(query.value(i).toString());
//               tablewiget->setItem(row,i,tabwdt_itm);

//           }

//           ++row;
           doAfterQueryExe(query);
       }
   }
   else
   {
       QSqlError sqlError = query.lastError();

       if(sqlError.type() == QSqlError::UnknownError)
       {
           qDebug() << "unkonw error";
           QMessageBox::information(parent, "提示信息", "未知错误");
       }
       else if(sqlError.type() == QSqlError::StatementError)
       {
           qDebug() << "lag error";
       }
   }
}

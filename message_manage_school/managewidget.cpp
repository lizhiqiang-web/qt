#include "managewidget.h"
#include "ui_managewidget.h"

ManageWidget::ManageWidget(QSqlDatabase &database,QWidget *parent) :
    QMainWindow(parent),db(database),
    ui(new Ui::ManageWidget)
{
    ui->setupUi(this);
    setFixedSize(1260,680);
    ui->treeWidget->setHeaderHidden(true);
    ui->stackedWidget->setCurrentWidget(ui->page0);

//  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);


    QString sql = "select * from student_msg;";
    showSqlSelectedIitem(sql,ui->tableWidget);


    connect(ui->tableWidget, &QTableWidget::cellClicked, this,[=](int row, int column)mutable{

            qDebug() << row <<","<< column;

    });
    //把数据库的数据导入表格中;
}


ManageWidget::~ManageWidget()
{

    delete ui;
}

 void ManageWidget::setCurRow_isEidt(QTableWidget *tb_wdt, bool en)
{
    if(tb_wdt == NULL)
    {
        return;
    }
    int row = tb_wdt->currentRow();
    int col_cnt = tb_wdt->columnCount();
    Qt::ItemFlags falgs ;
    for (auto i = 0; i < col_cnt; i++)
    {
        auto *item = tb_wdt->item(row, col_cnt);
        if(item == NULL)
        {
            item = new QTableWidgetItem();
        }
        falgs = item->flags();
        if(en == true)
        {
             falgs |=   Qt::ItemIsEditable;
        }
        else
        {
            falgs &=   ~Qt::ItemIsEditable;
        }
        item->setFlags(falgs);
    }
}


void ManageWidget::showSqlSelectedIitem(QString sql,QTableWidget * tablewiget)
{
   QSqlQuery query;
   if(query.exec(sql))
   {
       qDebug()<<query.size();
       int row = 0;
       tablewiget->setRowCount(0);
       while(query.next())
       {

           tablewiget->setRowCount(row + 1);
           for (auto i =0 ; i < tablewiget->columnCount(); i++)
           {
               QTableWidgetItem *tabwdt_itm = new QTableWidgetItem();
               Qt::ItemFlags falgs = tabwdt_itm->flags() & ~Qt::ItemIsEditable;
               tabwdt_itm->setFlags(falgs);
               tabwdt_itm->setText(query.value(i).toString());
               tablewiget->setItem(row,i,tabwdt_itm);
           }

           ++row;
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
void ManageWidget::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "text" << item->text(column);

    if(item->text(column) == "学生信息")
    {
        ui->stackedWidget->setCurrentWidget(ui->stu_msg);
    }
    else if(item->text(column) == "老师信息")
    {

        ui->stackedWidget->setCurrentWidget(ui->tch_msg);
    }
    else if(item->text(column) == "职工信息")
    {

        ui->stackedWidget->setCurrentWidget(ui->work_msg);
    }
    else if(item->text(column) == "学生成绩")
    {

        ui->stackedWidget->setCurrentWidget(ui->stu_score);
    }
    else if(item->text(column) == "老师成绩")
    {

        ui->stackedWidget->setCurrentWidget(ui->tch_score);
    }
    else if(item->text(column) == "老师工资")
    {
        ui->stackedWidget->setCurrentWidget(ui->tch_salary);
    }
    else if(item->text(column) == "职工工资")
    {

        ui->stackedWidget->setCurrentWidget(ui->work_salary);
    }


}

void ManageWidget::on_updata_smsg_clicked()
{
    static int row = -1;

    int column = ui->tableWidget->columnCount();
    static QStringList sava_str;
    if(ui->updata_smsg->text() == "修改")
    {
        row = ui->tableWidget->currentRow();
        if(row < 0)
        {
            return;
        }
        ui->find_smeg->setEnabled(false);
        ui->del_smsg->setEnabled(false);
        ui->add_msg->setEnabled(false);
         sava_str.clear();
         for (auto i = 0; i < column; i++)
         {
             QTableWidgetItem *item = ui->tableWidget->item(row,i);
             sava_str << item->text();
             setItem_isEidt(item);
         }
         ui->updata_smsg->setText("保存/取消");
         QMessageBox::information(this, "提示", "对当前行进行修改在点击保存/取消");
     }
     else
     {
                ui->find_smeg->setEnabled(true);
                ui->del_smsg->setEnabled(true);
                ui->add_msg->setEnabled(true);
                ui->updata_smsg->setText("修改");
                QStringList list;
                for (auto i = 0; i < column; i++)
                {

                     QTableWidgetItem *item = ui->tableWidget->item(row,i);
                     setItem_isEidt(item,false);
                     list << item->text();

                }
                int ret = QMessageBox::question(this, "提示", "是否保存",QMessageBox::Save | QMessageBox::Cancel);
                if(ret == QMessageBox::Save)
                {
                    //数据库操作
                    QString id = list.takeFirst();
                    QString sql = "delete from student_msg where id = " + id + ";";

                    bool ret = exeChangeDataBaseSql(sql,this->db,this);
                    if(ret == true)
                    {
                        sql = "insert into student_msg values(" + id + ",\'" + list.join("\',\'")  + "\', \'\',\'\',\'\',\'\',\'\');";
                        ret = exeChangeDataBaseSql(sql,this->db,this);
                        if(ret == true)
                        {
                            return;
                        }
                    }
                }
                for (auto i = 0; i < column; i++)
                {

                     QTableWidgetItem *item = ui->tableWidget->item(row,i);
                     item->setText(sava_str.at(i));
                }


    }
}

void ManageWidget::on_del_smsg_clicked()
{

    int row = ui->tableWidget->currentRow();
    if(row < 0)
    {
        return;
    }
     int ret = QMessageBox::question(this, "提问", "确定删除吗？");
     if(ret == QMessageBox::Yes)
     {
         QString id = ui->tableWidget->item(row,0)->text();
         QString sql = "delete from student_msg where id = " + id + ";";
         int ret = exeChangeDataBaseSql(sql,this->db,this);
         if(ret == true)
         {
             ui->tableWidget->removeRow(row);
         }
    }
}


void ManageWidget::on_find_smeg_clicked()
{

    if(ui->find_smeg->text() == "查找")
    {

        QString id = ui->id_edt->text();
        QString name = ui->name_edt->text();

        QString sql;
        if(id == NULL && name == NULL)
        {
            return;
        }
        else if(id == NULL && name != NULL)
        {
            qDebug()<<"id k";
            sql = "select * from student_msg where name = " + name + ";";
        }
        else if(id != NULL && name == NULL)
        {
            qDebug()<<"name k";
            sql = "select * from student_msg where id = " + id + ";";
        }
        else if(id != NULL && name != NULL)
        {
            qDebug()<<"all k";
            sql = "select * from student_msg where id = " + id + " and name = " + name + ";";
        }
        //数据库操作
        ui->id_edt->setEnabled(false);
        ui->name_edt->setEnabled(false);
        ui->find_smeg->setText("返回");
        ui->tableWidget->clearContents();
        showSqlSelectedIitem(sql,ui->tableWidget);

    }
    else
    {
        ui->find_smeg->setText("查找");
        ui->id_edt->setEnabled(true);
        ui->name_edt->setEnabled(true);
        //数据库操作

        QString sql = "select * from student_msg;";
        showSqlSelectedIitem(sql,ui->tableWidget);
    }
}



void ManageWidget::on_add_msg_clicked()
{
    static int row = -1;

    int column = ui->tableWidget->columnCount();

    if(ui->add_msg->text() == "添加")
    {
        row = ui->tableWidget->currentRow();
        if(row < 0)
        {
            return;
        }
        ui->find_smeg->setEnabled(false);
        ui->del_smsg->setEnabled(false);
        ui->updata_smsg->setEnabled(false);

        ui->tableWidget->insertRow(row);
        for (auto i = 0; i < column; i++)
        {

             QTableWidgetItem *item = new QTableWidgetItem();
                setItem_isEidt(item);
             ui->tableWidget->setItem(row,i,item);
        }


        ui->add_msg->setText("保存/取消");
        QMessageBox::information(this, "提示", "对当前行进行修改在点击保存/取消");
    }
    else
    {
            QStringList list;
            for (auto i = 0; i < column; i++)
            {
                 QTableWidgetItem *item = ui->tableWidget->item(row,i);
                 list << item->text();
            }
           int ret = QMessageBox::question(this, "提示", "是否保存",QMessageBox::Save | QMessageBox::Cancel);
           if(ret == QMessageBox::Save)
           {
                //数据库操作
               QString id = list.takeFirst();

               QString sql = "insert into student_msg values(" + id + ",\'" + list.join("\',\'")  + "\', \'\',\'\',\'\',\'\',\'\');";

               qDebug() << sql;
               bool ret = exeChangeDataBaseSql(sql,this->db,this);
               if(ret == true)
               {

               }
               else
               {
                   QMessageBox::information(this, "提示", "填入格式错误请重试");
                   return;
               }
           }
           else
           {
                 ui->tableWidget->removeRow(row);
           }
           ui->add_msg->setText("添加");
           ui->find_smeg->setEnabled(true);
           ui->del_smsg->setEnabled(true);
           ui->updata_smsg->setEnabled(true);

           for (auto i = 0; i < column; i++)
           {

                QTableWidgetItem *item = ui->tableWidget->item(row,i);
                setItem_isEidt(item, false);
                list << item->text();
           }
           row = -1;
    }
}

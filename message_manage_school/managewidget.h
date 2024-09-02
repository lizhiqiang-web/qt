#ifndef MANAGEWIDGET_H
#define MANAGEWIDGET_H

#include <QMainWindow>

#include "publicinfo.h"
namespace Ui {
class ManageWidget;
}

class ManageWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManageWidget(QSqlDatabase &database,QWidget *parent = nullptr);
    ~ManageWidget();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_updata_smsg_clicked();

    void on_del_smsg_clicked();

    void on_find_smeg_clicked();


    void on_add_msg_clicked();



private:
    Ui::ManageWidget *ui;

    QSqlDatabase &db;



     void showSqlSelectedIitem(QString sql,QTableWidget * tablewiget);

     void setCurRow_isEidt(QTableWidget *tb_wdt, bool en = true);
     void setItem_isEidt(QTableWidgetItem *item, bool en = true)
     {
         if(item == NULL)
         {
             return;
         }
         Qt::ItemFlags falgs = item->flags();
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



};


#endif // MANAGEWIDGET_H

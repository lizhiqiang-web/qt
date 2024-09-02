#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QWidget>

#include "publicinfo.h"
namespace Ui {
class StudentWidget;
}

class StudentWidget : public QWidget
{
    Q_OBJECT

public:
     StudentWidget(QSqlDatabase &database,int id,QWidget *parent = nullptr);
    ~StudentWidget();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);


    void on_updata_btn_clicked();

private:
    Ui::StudentWidget *ui;
    QSqlDatabase &db;
    int id;
    void showSqlSelectedIitem(QString sql);
    void setLineEidtsORead(bool en);


};

#endif // STUDENTWIDGET_H

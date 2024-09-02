#ifndef PUBLIC_INFO_H
#define PUBLIC_INFO_H
#include <memory>
#include <QtDebug>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QMainWindow>
void initDataBase(QSqlDatabase &db, QWidget *parent = nullptr);

bool exeChangeDataBaseSql(QString sql,QSqlDatabase &db, QWidget *parent = nullptr);
using doAfterQueryExe = void(*)(QSqlQuery &query);
void showSqlSelectedIitem(QString sql,QWidget *parent,doAfterQueryExe);
#endif


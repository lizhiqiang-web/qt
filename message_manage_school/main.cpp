#include "login.h"
#include "managewidget.h"
#include "studentwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login w;
    w.show();

//    ManageWidget mw;
//    mw.show();


    return a.exec();
}

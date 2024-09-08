#include "login_wdt.h"
#include "chatwdt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWdt w;



     w.show();
    return a.exec();
}

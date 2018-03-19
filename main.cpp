#include "smail.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sMail w;
    if(w.getUserListFromDB())
    {
        w.login();
    }
    else
    {
        w.addUser();
    }

    return a.exec();
}

#include "smail.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sMail w;
    if(w.getUserList().isEmpty())
    {
        w.getAccountManager()->show();
    }
    else
    {
        w.getMailViewer()->show();
    }

    return a.exec();
}

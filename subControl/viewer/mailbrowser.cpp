#include "mailbrowser.h"
#include <QVBoxLayout>

MailBrowser::MailBrowser(QWidget *parent) : QScrollArea(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
}

void MailBrowser::setMailList(const QList<QString> &mails)
{
    for(auto i : mails)
    {
        QWebView *view = new QWebView(this);

        //view->resize(this->widget());
    }


}

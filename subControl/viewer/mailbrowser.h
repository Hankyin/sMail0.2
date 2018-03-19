#ifndef MAILBROWSER_H
#define MAILBROWSER_H

#include <QWidget>
#include <QScrollArea>
#include <QWebView>

class MailBrowser : public QScrollArea
{
    Q_OBJECT
public:
    explicit MailBrowser(QWidget *parent = nullptr);
    void setMailList(const QList<QString> &mails);
signals:

public slots:

private:
    QList<QString> mailList;//来自同一个发件人的邮件
    QList<QWebView*> viewList;
};

#endif // MAILBROWSER_H

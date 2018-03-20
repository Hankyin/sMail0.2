#ifndef MAILBROWSER_H
#define MAILBROWSER_H

#include <QWidget>
#include <QStackedWidget>
#include <QWebView>
#include <QLineEdit>

class MailBrowser : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MailBrowser(QWidget *parent = nullptr);
    void setMailList(const QList<QString> &mails);
    void setMail(const QString &mail);
signals:

public slots:

private slots:
    void openExternalLink(QUrl url);
    void openMail(int index);
private:
    QLineEdit *from;
    QLineEdit *to;
    QLineEdit *subject;
    QWebView *webView;

    QWidget *browser;
    QWidget *conversation;

    QList<QString> mailList;
};

#endif // MAILBROWSER_H

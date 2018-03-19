#ifndef MAILVIEWER_H
#define MAILVIEWER_H

#include <QWidget>
#include <QWebView>
#include <qcef/qcef_web_view.h>
#include "subControl/viewer/sidebar.h"
#include "subControl/viewer/liststack.h"
#include "netProtocol/imap.h"

//class sMail;
class MailSender;

class MailViewer : public QWidget
{
    Q_OBJECT

public:
    MailViewer(UserInfo user);
    ~MailViewer();
    void setMailList(const QList<MailInfo> &mailList);
    void setDirList(const QList<DirInfo> &dirList);
    void setUserList(const QList<UserInfo> &userList);

private slots:
    void toMailList();
    void toUserList();
    void toDirList();
    void writeMail();

    void IMAPError(QString error);
    void IMAPConnected();
    void IMAPLoginFinish();
    void IMAPListFinish(QList<IMAPDir> dirs);
    void IMAPSelectFinish(QString dir,uint mails,uint recents);
    void IMAPFetchFinsh(int index,QString mail);
    void IMAPSearchFinish(QList<int> searchList);

    void showMail(int i);
    void changeDir(QString dir);
private:
    SideBar *sideBar;
    ListStack *listStack;
    QWebView *webView;
    IMAP *imap;
    UserInfo user;
    QString curDir;
    QList<QString> mails;
    QList<MailInfo> headList;
    MailSender *mailSender;
};

#endif // MAILVIEWER_H

#ifndef MAILSENDER_H
#define MAILSENDER_H


#include <QWidget>
#include "subControl/sender/lineedit.h"
#include "subControl/sender/maileditor.h"
#include "subControl/sender/cmdbar.h"
#include "subControl/sender/emojipicker.h"
#include "utils/translator.h"
#include "utils/type.h"

class MailSender : public QWidget
{
    Q_OBJECT

public:
    MailSender(UserInfo user);
    ~MailSender();
private slots:
    void addAttachment();
    void translate(const QString language);
    void setTranslation(const QString result);
    void showEmojiPicker();
    void sendMail();
private:
    LineEdit *from;
    LineEdit *to;
    LineEdit *subject;
    MailEditor *editor;
    CmdBar *cmdBar;
    EmojiPicker *emojiPicker;
    Translator *translator;
    UserInfo user;
    QList<QString> attList;
};

#endif // MAILSENDER_H

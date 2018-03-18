#ifndef MAILSENDER_H
#define MAILSENDER_H


#include <QWidget>
#include "subControl/lineedit.h"
#include "subControl/maileditor.h"
#include "subControl/cmdbar.h"
#include "subControl/emojipicker.h"
#include "utils/translator.h"

class MailSender : public QWidget
{
    Q_OBJECT

public:
    MailSender(QWidget *parent = 0);
    ~MailSender();
private slots:
    void addAttachment();
    void translate(const QString language);
    void setTranslation(const QString result);
    void showEmojiPicker();
private:
    LineEdit *from;
    LineEdit *to;
    LineEdit *subject;
    MailEditor *editor;
    CmdBar *cmdBar;
    EmojiPicker *emojiPicker;
    Translator *translator;
};

#endif // MAILSENDER_H

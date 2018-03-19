#include "mailsender.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QStringListModel>
#include <QCompleter>
#include <QPushButton>
#include <QFileDialog>//获取附件
#include <QDir>
#include "utils/translator.h"
#include "utils/mime.h"
#include "netProtocol/smtp.h"

MailSender::MailSender(UserInfo user)
    : QWidget(0)
{
    this->user = user;
    from = new LineEdit(tr("发件人:"));
    to = new LineEdit(tr("收件人:"),true);
    subject = new LineEdit(tr("主题： "));
    from->setReadOnly(true);
    from->setFocusPolicy(Qt::NoFocus);
    QStringListModel *model = new QStringListModel;
    QStringList list;
    list<<"yin <loufand@163.com>"<<"微软 <store@microsoft.com"<<"loufand@outlook.com";
    model->setStringList(list);
    QCompleter *toCompleter = new QCompleter(model);
    toCompleter->setFilterMode(Qt::MatchContains);
    to->setComplater(toCompleter);
    to->setFocus();
    editor = new MailEditor;
    translator = new Translator(this);
    cmdBar = new CmdBar;
    emojiPicker = nullptr;
    cmdBar->setTranslateSupportLang(translator->getLanguageList());
    QVBoxLayout *contentLayout = new QVBoxLayout();
    contentLayout->addWidget(from);
    contentLayout->addWidget(to);
    contentLayout->addWidget(subject);
    contentLayout->addWidget(editor);
    contentLayout->addWidget(cmdBar);
    this->setLayout(contentLayout);
    this->resize(400,500);

    connect(this->cmdBar,SIGNAL(sendMailClick()),this,SLOT(sendMail()));
    connect(this->cmdBar,SIGNAL(attachmentClick()),this,SLOT(addAttachment()));
    connect(this->cmdBar,SIGNAL(translateClick(QString)),this,SLOT(translate(QString)));
    connect(this->cmdBar,SIGNAL(emojiClick()),this,SLOT(showEmojiPicker()));
    connect(this->translator,SIGNAL(translateResult(QString)),this,SLOT(setTranslation(QString)));
}

MailSender::~MailSender()
{

}

void MailSender::addAttachment()
{
    QString attachmentPath = QFileDialog::getOpenFileName(this,"选择附件",QDir::homePath());
    if(attachmentPath.isEmpty())
        return;
    this->editor->addItem(attachmentPath);
    this->attList.append(attachmentPath);

}

void MailSender::translate(const QString language)
{
    translator->requireTranslate(this->editor->toPlainText(),language);
}

void MailSender::setTranslation(const QString result)
{
    if(result.isEmpty())
        return;
    this->editor->setText(result);
}

void MailSender::showEmojiPicker()
{
    if(emojiPicker == nullptr)
    {
        emojiPicker = new EmojiPicker(this);
    }
    emojiPicker->show();

}

void MailSender::sendMail()
{
    QString from = this->from->text();
    QString mimeFrom = MIME::HeadEncoding(user.name.toUtf8(),MIME::UTF8,false) + " <" + from + ">";
    QList<QString> rcptTo = this->to->getItemList();
    QString mimeTo;
    for(int i = 0;i < rcptTo.size();i++)
    {
        QString t;
        t.append('<');
        t.append(rcptTo.at(i));
        t.append('>');
        mimeTo.append(t);
    }
    QString subject = this->subject->text();
    QDateTime date = QDateTime::currentDateTime();
    QString htmlContent = this->editor->toHtml();
    QString plainContent = this->editor->toPlainText();
    MIMEMultipart mixed(MIME::multipart_mixed);
    mixed.addHead("From",mimeFrom.toUtf8());
    mixed.addHead("To",mimeTo.toUtf8());
    mixed.addHead("Subject",mixed.HeadEncoding(subject.toUtf8(),MIME::UTF8,false));
    mixed.addHead("Date",date.toString(Qt::RFC2822Date).toUtf8());
    mixed.addHead("Mime-Version","1.0");
    MIMEMultipart alternative(MIME::multipart_alternative);
    MIMEText msgText(plainContent.toUtf8(),MIME::text_plain,0);
    MIMEText msgHtml(htmlContent.toUtf8(),MIME::text_html,0);
    alternative.append(msgHtml);
    alternative.append(msgText);
    mixed.append(alternative);

    //插入附件
    for(auto a : this->attList)
    {
        if(a == nullptr)
            continue;
        QString fn = a;
        int fti = fn.lastIndexOf('.');
        QString ft = fn.mid(fti);
        if(ft.toLower() == "jpeg" || ft.toLower() == "jpg")
        {
            MIMEImage att(fn,MIME::image_jpeg,true);
            mixed.append(att);
        }
        else if(ft.toLower() == "png")
        {
            MIMEImage att(fn,MIME::image_png,true);
            mixed.append(att);
        }
        else if(ft.toLower() == "pdf")
        {
            MIMEApplication att(fn,MIME::application_pdf);
            mixed.append(att);
        }
        else if(ft.toLower() == "zip")
        {
            MIMEApplication att(fn,MIME::application_zip);
            mixed.append(att);
        }
        else
        {
            MIMEApplication att(fn,MIME::application_octet_stream);
            mixed.append(att);
        }
    }

        SMTP *smtp = new SMTP(this);
        smtp->setDebugMode(true);
        smtp->setServerAddr(user.SMTPServer,user.SMTPPort,user.isSMTPSSL);
        smtp->setLoginInfo(user.mail,user.pass);
        smtp->sendMail(rcptTo,mixed.getContent());
}

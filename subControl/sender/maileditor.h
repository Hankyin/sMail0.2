#ifndef MAILEDITER_H
#define MAILEDITER_H

#include <QScrollArea>
#include <QWidget>
#include <QTextEdit>
#include <QList>
#include "itembutton.h"
#include "utils/flowlayout.h"

class MailEditor : public QScrollArea
{
    Q_OBJECT
public:
    explicit MailEditor(QWidget *parent = nullptr);
    QString toPlainText() const {return this->textEdit->toPlainText();}
    QString toHtml() const {return this->textEdit->toHtml();}
    void setText(const QString &text) {this->textEdit->setText(text);}
signals:

public slots:
    void addItem(QString &attachmentPath);
    void delItem(uint id);
    void resizeTextEditHeight();
private:
    QTextEdit *textEdit;
    QWidget *attachmentArea;
    QWidget *bgArea;
    QList<ItemButton*> attachmentList;
    FlowLayout *attachmentLayout;
};

#endif // MAILEDITER_H

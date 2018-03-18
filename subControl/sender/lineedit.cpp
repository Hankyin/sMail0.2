#include "lineedit.h"
#include "itembutton.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QDateTime>//用来表示ItemButton的ID
#include <QRegExp>
#include <QMessageBox>
#include <QDebug>

LineEdit::LineEdit(QString title, bool enableItem, QWidget *parent) : QWidget(parent)
{
    label = new QLabel(this);
    label->setText(title);
    label->setStyleSheet("color: rgb(145, 143, 144);"
                         "font: 10pt;");
    edit = new QLineEdit(this);
    edit->setStyleSheet("background:transparent;"
                        "border-width:0;"
                        "border-style:outset;"
                        "font: 10pt");
    editLayout = new QHBoxLayout(this->edit);
    editLayout->addStretch();
    editLayout->setSpacing(0);
    editLayout->setMargin(0);
    edit->setLayout(editLayout);
    line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(label);
    hLayout->addWidget(edit);
    hLayout->setMargin(0);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(line);
    vLayout->setMargin(5);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocusProxy(this->edit);
    if(enableItem)
    {
        connect(this->edit,SIGNAL(editingFinished()),this,SLOT(addItem()));
        connect(this,SIGNAL(itemCountChange()),this,SLOT(resizeTextMargins()));
    }
}

void LineEdit::setComplater(QCompleter *complater)
{
    //connect(complater,SIGNAL(activated(QString)),this,SLOT(addItem()));
    this->edit->setCompleter(complater);
}

void LineEdit::addItem()
{
    QString str = this->edit->text();
    if(str.isEmpty())
        return;
    this->edit->setText("");
    //输入字符串的处理
    QString mail;
    QString name;
    QRegExp re("<\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*>");//邮箱地址匹配
    QRegExp re2("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");

    QStringList strList = str.split(' ',QString::SkipEmptyParts);
    if(strList.size() == 2 && re.exactMatch(strList.at(1)))
    {
        //标准格式 name <mail@aaa.ccc>
        name = strList.at(0);
        mail = strList.at(1);
    }
    else if(re.exactMatch(str) || re2.exactMatch(str))
    {
        //mail@aaa.ccc or <mail@aaa.ccc>
        name = str;
    }
    else
    {
        QMessageBox::warning(this,"错误","邮箱地址格式错误");
        return;
    }

    uint newID = (QDateTime::currentDateTime().toTime_t() << 2) + editLayout->count();
    ItemButton *item = new ItemButton(newID,name);
    item->setData(mail);
    connect(item,SIGNAL(remove(uint)),this,SLOT(delItem(uint)));
    this->itemBtList.append(item);
    editLayout->insertWidget(editLayout->count()-1,item);
    emit itemCountChange();
}

void LineEdit::delItem(uint id)
{
    //get the widget wanted to delate
    ItemButton *item = Q_NULLPTR;
    for(auto i : itemBtList)
    {
        if(i->getID() == id)
        {
            item = i;
        }
    }
    if(!item)
        return;

    editLayout->removeWidget(item);
    itemBtList.removeOne(item);
    delete item;
    emit itemCountChange();
}

void LineEdit::resizeTextMargins()
{
    int lw = 0;
    for(auto i : itemBtList)
    {
        lw += i->width();
    }
    this->edit->setTextMargins(lw,0,0,0);
}


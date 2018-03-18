#include "itembutton.h"
#include <QMenu>
#include <QAction>
#include <QFontMetrics>

ItemButton::ItemButton(uint id, const QString &text, QWidget *parent)
    :QPushButton(text, parent)
{
    this->id = id;
    this->data = text;
    menu = new QMenu;
    QAction *actRemove = new QAction(tr("移除"),menu);
    menu->addAction(actRemove);
    this->setMenu(menu);
    //this->setFlat(true);
    this->addAction(actRemove);
    this->setStyleSheet("text-align: left;"
                        "padding-left: 5px");
    int w = this->fontMetrics().width(text);
    this->setFixedWidth(w + 30);//30指的是按钮右边的菜单按钮，这是估计值。
    connect(actRemove,SIGNAL(triggered(bool)),this,SLOT(del()));
}

void ItemButton::addAction(QAction *action)
{
    this->menu->insertAction(menu->actions().last(),action);
}

QString ItemButton::getData() const
{
    return data;
}

void ItemButton::setData(const QString &value)
{
    data = value;
}

void ItemButton::setIcon(const QIcon &icon)
{
    QAbstractButton::setIcon(icon);
    this->setFixedWidth(this->width() + 20);//20代表图标的宽度
}

void ItemButton::del()
{
    emit remove(this->id);
}



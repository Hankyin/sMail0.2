#include "sidebutton.h"
#include <QIcon>

SideButton::SideButton(const QString icon, QWidget *parent) : QToolButton(parent)
{
    setAutoRaise(true);
    setFixedSize(BUTTON_SIZE,BUTTON_SIZE);
    setIcon(QIcon(icon));
    setIconSize(this->size() - QSize(10,10));
}


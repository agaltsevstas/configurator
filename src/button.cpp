#include "button.h"

Button::Button(const QString &name, QWidget *parent) :
    QPushButton(parent),
    name_      (name)
{
    setText(name_);                                                // Установка имени
    setCursor(QCursor(Qt::PointingHandCursor));                    // Изменение курсора при наведении
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); // Изменение ширины и высоты при масштабировании
    QPalette palette;
    QBrush green(QColor(78, 154, 6, 255));                         // Кисть зеленого цвета
    QBrush white(QColor(255, 255, 255, 255));                      // Кисть белого цвета
    palette.setBrush(QPalette::Active, QPalette::Button, green);   // Установка цвета для активного состояния
    palette.setBrush(QPalette::Inactive, QPalette::Button, green); // Установка цвета для неактивного состояния
    palette.setBrush(QPalette::Disabled, QPalette::Button, white); // Установка цвета для отключенного состояния
    setPalette(palette);
}

void Button::mousePressEvent(QMouseEvent *event)
{
    if      (event->button() == Qt::RightButton) emit rightClicked();
    else if (event->button() == Qt::LeftButton)  emit leftClicked();
    else if (event->button() == Qt::MidButton)   emit midClicked();
}

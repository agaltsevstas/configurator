#include "graphics_view.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);          // Отключение скроллбар по горизонтали
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);            // Отключение скроллбар по вертикали
    setAlignment(Qt::AlignCenter);                                 // Привязка содержимого к центру
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); // Изменение ширины и высоты при масштабировании
    setSceneRect(QRectF(_x, _y, _width, _height));                 // Установка размеров виджета и сцены
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    // Масштабирование элементов на сцене
    fitInView(QRectF(_x, _y, _width / 2, _height / 2), Qt::KeepAspectRatio);

    QGraphicsView::resizeEvent(event);
}

#include "graphics_view.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);          // Отключение скроллбар по горизонтали
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);            // Отключение скроллбар по вертикали
    setAlignment(Qt::AlignCenter);                                 // Привязка содержимого к центру
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); // Изменение ширины и высоты при масштабировании
    setSceneRect(QRectF(x_, y_, width_, height_));                 // Установка размеров виджета и сцены
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    // Масштабирование элементов на сцене
    fitInView(QRectF(x_, y_, width_ / 2, height_ / 2), Qt::KeepAspectRatio);

    QGraphicsView::resizeEvent(event);
}

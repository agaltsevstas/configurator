#include <QGraphicsScene>
#include <QPainter>

#include "item.h"

Item::Item(Button *button, QGraphicsItem *parent) :
    QGraphicsPolygonItem (parent),
    name_ (button->getName()),
    button_ (button)
{
    // Отрисовка фигуры в виде многоугольника
    polygon_ << QPointF(x_, y_)
             << QPointF(width_ / 2, y_)
             << QPointF(width_ / 2,  height_ / 2)
             << QPointF(x_, height_ / 2)
             << QPointF(x_, y_);

    setPolygon(polygon_);
    setFlag(ItemIsMovable, true);            // Перемещение элемента
    setFlag(ItemIsSelectable, true);         // Выделение элемента
    setFlag(ItemSendsGeometryChanges, true); // Изменение расположения элемента
    setAcceptHoverEvents(true);              // События перемещения мыши над элементом
}

void Item::setEnabledButton()
{
    button_->setEnabled(true);
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, Qt::SolidLine)); // Установка цвета для надписи
    painter->setBrush(Qt::lightGray);  // Установка цвета для фона
    painter->drawRect(QRect(x_, y_, width_, height_));
    painter->drawText(QRect(x_, y_, width_, height_), Qt::AlignCenter, name_);
}

QRectF Item::boundingRect() const
{
    return QRect(x_, y_, width_, height_);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mouseCoords_ = pos() - mapToScene(event->pos());
    setCursor(QCursor(Qt::ClosedHandCursor)); // Изменение курсора при держании
}

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Транслирование координат курсора в координатную систему графической сцены
    setPos(mapToScene(event->pos() + mouseCoords_));
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    // Изменение курсора при отпускании
    setCursor(QCursor(Qt::ArrowCursor));
}

void Item::addArrow(Arrow *arrow)
{
    arrows_.append(arrow);
}

void Item::removeArrow(Arrow *arrow)
{
    int index = arrows_.indexOf(arrow);

    if (index != -1)
        arrows_.removeAt(index);
}

void Item::removeArrows()
{
    foreach (Arrow *arrow, arrows_)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

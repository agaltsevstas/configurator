#include "item.h"

#include <QGraphicsScene>
#include <QPainter>

Item::Item(Button *button, QGraphicsItem *parent) :
    QGraphicsPolygonItem (parent),
    _name(button->getName()),
    _button(button)
{
    // Отрисовка фигуры в виде многоугольника
    _polygon << QPointF(_x, _y)
             << QPointF(_width / 2, _y)
             << QPointF(_width / 2, _height / 2)
             << QPointF(_x, _height / 2)
             << QPointF(_x, _y);

    setPolygon(_polygon);
    setFlag(ItemIsMovable, true);            // Перемещение элемента
    setFlag(ItemIsSelectable, true);         // Выделение элемента
    setFlag(ItemSendsGeometryChanges, true); // Изменение расположения элемента
    setAcceptHoverEvents(true);              // События перемещения мыши над элементом
}

void Item::setEnabledButton()
{
    _button->setEnabled(true);
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, Qt::SolidLine)); // Установка цвета для надписи
    painter->setBrush(Qt::lightGray);  // Установка цвета для фона
    painter->drawRect(QRect(_x, _y, _width, _height));
    painter->drawText(QRect(_x, _y, _width, _height), Qt::AlignCenter, _name);
}

QRectF Item::boundingRect() const
{
    return QRect(_x, _y, _width, _height);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _mouseCoords = pos() - mapToScene(event->pos());
    setCursor(QCursor(Qt::ClosedHandCursor)); // Изменение курсора при держании
}

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Транслирование координат курсора в координатную систему графической сцены
    setPos(mapToScene(event->pos() + _mouseCoords));
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    // Изменение курсора при отпускании
    setCursor(QCursor(Qt::ArrowCursor));
}

void Item::addArrow(Arrow *arrow)
{
    _arrows.append(arrow);
}

void Item::removeArrow(const Arrow *arrow)
{
    int index = _arrows.indexOf(arrow);

    if (index != -1)
        _arrows.removeAt(index);
}

void Item::removeArrows()
{
    foreach (Arrow *arrow, _arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

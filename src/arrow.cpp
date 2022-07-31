#include "arrow.h"
#include "item.h"

#include <QPen>
#include <QPainter>

#include <math.h>

Arrow::Arrow(uint id, Item *startItem, Item *endItem) :
    _startItem (startItem),
    _endItem (endItem)  ,
    _id(id)
{
    setFlag(ItemIsSelectable, true);
    setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Если начальный элемент накладывается с конечным, то выходим
    if (_startItem->collidesWithItem(_endItem))
        return;

    QPen myPen = pen();
    myPen.setColor(_color);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(_color);

    QLineF centerLine(_startItem->pos(), _endItem->pos());
    QPolygonF endPolygon = _endItem->getPolygon();
    QPointF p1 = endPolygon.first() + _endItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;

    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon[i] + _endItem->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
                polyLine.intersects(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, _startItem->pos()));

    const qreal pi = 3.14;
    double angle = std::acos(line().dx() / line().length());

    if (line().dy() >= 0)
    {
        angle = (pi * 2) - angle;
    }
    
    QPointF arrowP1 = line().p1() + QPointF(sin(angle + pi / 3) * arrowSize,
                                    cos(angle + pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + pi - pi / 3) * arrowSize,
                                    cos(angle + pi - pi / 3) * arrowSize);

    _arrowHead.clear();
    _arrowHead << line().p1() << arrowP2 << arrowP1;
    painter->drawLine(line());
    painter->drawPolygon(_arrowHead);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(_arrowHead);
    return path;
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), 
           QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(_startItem, 0, 0), mapFromItem(_endItem, 0, 0));
    setLine(line);
}

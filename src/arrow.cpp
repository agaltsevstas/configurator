#include <QPen>
#include <QPainter>

#include <math.h>

#include "arrow.h"
#include "item.h"

Arrow::Arrow(uint id, Item *startItem, Item *endItem) :
    startItem_ (startItem),
    endItem_   (endItem)  ,
    id_        (id)
{
    setFlag(ItemIsSelectable, true);
    setPen(QPen(color_, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Если начальный элемент накладывается с конечным, то выходим
    if (startItem_->collidesWithItem(endItem_))
        return;

    QPen myPen = pen();
    myPen.setColor(color_);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(color_);

    QLineF centerLine(startItem_->pos(), endItem_->pos());
    QPolygonF endPolygon = endItem_->getPolygon();
    QPointF p1 = endPolygon.first() + endItem_->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;

    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon[i] + endItem_->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, startItem_->pos()));

    const qreal pi = 3.14;
    double angle = ::acos(line().dx() / line().length());

    if (line().dy() >= 0)
    {
        angle = (pi * 2) - angle;
    }
    
    QPointF arrowP1 = line().p1() + QPointF(sin(angle + pi / 3) * arrowSize,
                                    cos(angle + pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + pi - pi / 3) * arrowSize,
                                    cos(angle + pi - pi / 3) * arrowSize);

    arrowHead_.clear();
    arrowHead_ << line().p1() << arrowP2 << arrowP1;
    painter->drawLine(line());
    painter->drawPolygon(arrowHead_);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead_);
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
    QLineF line(mapFromItem(startItem_, 0, 0), mapFromItem(endItem_, 0, 0));
    setLine(line);
}

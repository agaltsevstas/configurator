#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

class Item;

/**
 * Класс Стрелка
 * Содержит название модуля и сигналы нажатия по кнопке.
 * Устанавливает связь между модулями.
 */
class Arrow : public QGraphicsLineItem
{

public:
    enum { Type = UserType + 2 }; /// Тип.

    /**
     * Конструктор
     * @param id Идентификатор стрелки.
     * @param startItem Начальный элемент.
     * @param endItem Конечный элемент.
     */
    Arrow(uint id, Item *startItem, Item *endItem);
    /// Получение типа.
    inline int type() const override { return Type; }
    /// Идентификатора стрелки.
    inline uint getId() const { return _id; }
    /// Получение указателя на начальный элемент.
    inline Item *startItem() const { return _startItem; }
    /// Получение указателя на конечный элемент.
    inline Item *endItem() const { return _endItem; }
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;

private:
    QPolygonF _arrowHead;       /// Голова стрелки (треугольник).
    QColor _color = Qt::black;  /// Цвет стрелки.
    Item *_startItem = nullptr; /// Указатель на начальный элемент.
    Item *_endItem   = nullptr; /// Указатель на конечный элемент.
    uint _id         = 0;       /// Идентификатор стрелки.

};

#endif // ARROW_H

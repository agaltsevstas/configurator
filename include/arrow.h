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
    inline uint getId() const { return id_; }
    /// Получение указателя на начальный элемент.
    inline Item *startItem() const { return startItem_; } 
    /// Получение указателя на конечный элемент.
    inline Item *endItem() const { return endItem_; }   
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QPainterPath shape() const override;
    QRectF       boundingRect() const override;

private:
    QPolygonF arrowHead_;         /// Голова стрелки (треугольник).
    QColor    color_ = Qt::black; /// Цвет стрелки.
    Item *startItem_ = nullptr;   /// Указатель на начальный элемент.
    Item *endItem_   = nullptr;   /// Указатель на конечный элемент.
    uint id_         = 0;         /// Идентификатор стрелки.

};

#endif // ARROW_H

#ifndef Item_H
#define Item_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "button.h"
#include "arrow.h"

/**
 * Класс Элемент
 * Содержит кнопку, входящие и исходящие стрелки.
 * Отрисовка фигуры.
 */
class Item : public QGraphicsPolygonItem
{

public:
    enum { Type = UserType + 1 }; /// Тип.

    /**
     * Конструктор
     * @param button Кнопка.
     */
    Item(Button *button, QGraphicsItem *parent = nullptr);
    /// Получение типа.
    inline int type() const override { return Type; }     
    /// Получение фигуры.
    inline const QPolygonF getPolygon() const { return polygon_; } 
    /// Получение названия.
    inline const QString   getName() const { return name_; }    
    void setEnabledButton();        /// Сделать кнопку доступной.
    void addArrow(Arrow *arrow);    /// Добавление стрелки.
    void removeArrow(Arrow *arrow); /// Удаление стрелки.
    void removeArrows();            /// Удаление всех стрелок.

protected:
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    void   mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void   mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void   mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
    const int x_ = -75; /// Расположение по x.
    const int y_ = -15; /// Расположение по y.
    const int width_  = 150; /// Ширина.
    const int height_ = 30;  /// Длина.
    
    QString name_;        /// Название.
    QPointF mouseCoords_; /// Координаты мыши.
    QPolygonF polygon_;   /// Отрисовка фигуры.
    Button *button_ = nullptr; /// Указатель на кнопку.
    QList<Arrow *> arrows_;    /// Список всех входящих и выходящих стрелок.
};

#endif // Item_H

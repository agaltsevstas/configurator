#ifndef Item_H
#define Item_H

#include "button.h"
#include "arrow.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

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
    inline const QPolygonF getPolygon() const { return _polygon; }
    /// Получение названия.
    inline const QString getName() const { return _name; }
    void setEnabledButton();        /// Сделать кнопку доступной.
    void addArrow(Arrow *arrow);    /// Добавление стрелки.
    void removeArrow(const Arrow *arrow); /// Удаление стрелки.
    void removeArrows();            /// Удаление всех стрелок.

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
    const int _x = -75; /// Расположение по x.
    const int _y = -15; /// Расположение по y.
    const int _width = 150; /// Ширина.
    const int _height = 30; /// Длина.
    
    QString _name;        /// Название.
    QPointF _mouseCoords; /// Координаты мыши.
    QPolygonF _polygon;   /// Отрисовка фигуры.
    Button *_button = nullptr; /// Указатель на кнопку.
    QList<Arrow *> _arrows;    /// Список всех входящих и выходящих стрелок.
};

#endif // Item_H

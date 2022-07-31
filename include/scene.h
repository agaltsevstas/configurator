#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QMap>

/**
 * Класс Сцена
 * Содержит элементы и стрелки.
 * Отрисовка фигур.
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene() {}
    inline void setIsChanged(bool isChanged) { _isChanged = isChanged; } /// Установка флага изменения сцены.
    inline bool getIsChanged() const { return _isChanged; }              /// Получение флага изменения сцены.
    /// Установка новых координат элемента при добавлении на сцену.
    inline QPointF coordinates()                                            
    { 
        _y += 50;
        return QPointF(_x, _y);
    }                    
    inline const QMap<uint, QPair<QString, QString>> getLinks() const { return _links; } /// Получение карты связей элементов.

signals:
    void arrowInserted(); /// Добавление стрелки.

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool _isChanged = false; /// Флаг изменения состояния сцены.
    qreal _x = 200; /// Расположение по x элемента на сцене.
    qreal _y = 0; /// Расположение по y элемента на сцене.
    uint _idArrow = 1; /// Идентификатор стрелки.
    QGraphicsLineItem *_line = nullptr; /// Линия между элементами.
    QMap<uint, QPair<QString, QString>> _links; /// Карта связей элементов.
};

#endif // SCENE_H

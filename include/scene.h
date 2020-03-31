#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

#include "arrow.h"
#include "item.h"

/**
 * Класс Сцена
 * Содержит элементы и стрелки.
 * Отрисовка фигур.
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent = nullptr) {}
    inline void setIsChanged(bool isChanged) { isChanged_ = isChanged; } /// Установка флага изменения сцены.
    inline bool getIsChanged() const { return isChanged_; }              /// Получение флага изменения сцены.
    /// Установка новых координат элемента при добавлении на сцену.
    inline QPointF coordinates()                                            
    { 
        y_ += 50; 
        return QPointF(x_, y_); 
    }                    
    inline const QMap<uint, QPair<QString, QString>> getLinks() const { return links_; } /// Получение карты связей элементов.

signals:
    void arrowInserted(); /// Добавление стрелки.

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool  isChanged_ = false; /// Флаг изменения состояния сцены.
    qreal x_         = 200;   /// Расположение по x элемента на сцене.
    qreal y_         = 0;     /// Расположение по y элемента на сцене.
    uint  idArrow_   = 1;     /// Идентификатор стрелки.
    QGraphicsLineItem *line_ = nullptr; /// Линия между элементами.
    QMap<uint, QPair<QString, QString>>  links_; /// Карта связей элементов.
};

#endif // SCENE_H

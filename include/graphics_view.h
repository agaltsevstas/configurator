#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H

#include <QGraphicsView>

/**
 * Виджет Класс
 * Отображает содержимое Scene.
 */
class GraphicsView : public QGraphicsView
{

public:
    GraphicsView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    const int x_      = 0;
    const int y_      = 0;
    const int width_  = 800; /// Ширина окна виджета и сцены
    const int height_ = 600; /// Высота окна виджета и сцены
};

#endif // GRAPHICS_VIEW_H

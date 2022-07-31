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
    void resizeEvent(QResizeEvent *event) override;

private:
    const int _x = 0;
    const int _y = 0;
    const int _width = 800;  /// Ширина окна виджета и сцены
    const int _height = 600; /// Высота окна виджета и сцены
};

#endif // GRAPHICS_VIEW_H

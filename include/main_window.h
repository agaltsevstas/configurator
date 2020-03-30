#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "button.h"
#include "item.h"
#include "scene.h"
#include "graphics_view.h"
#include "configurator.h"
#include "dialog_window.h"

namespace Ui
{
    class MainWindow;
}

/**
 * Класс Главное окно
 * Содержит кнопки, виджет со встроенной сценой.
 * Сохраняет сообщение.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Конструктор
     * @param filePaths Карта путей к модулям.
     * @param types Карта исходных состояний модулей.
     */
    MainWindow(map<string, string> &filePaths, map<string, bool> *types = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// Добавление кнопки на сцену
    void addButton();                    
    /// Сохранение всех связей между модулями в сообщение и переход к диалоговому окну.
    void on_generateYmlButton_clicked(); 
    /// Проверка на содержание хотя бы 1 связи между модулями.
    void timerAlarm();                   

private:
    map<string, string> filePaths_;
    Ui::MainWindow *mainwindow_ = nullptr; /// Главное окно
    GraphicsView *graphicsView_ = nullptr; /// Виджет
    DialogWindow *dialogWindow_ = nullptr; /// Диалоговое окно
    Scene *scene_  = nullptr; /// Сцена
    QTimer *timer_ = nullptr; /// Таймер
};

#endif // MAIN_WINDOW_H

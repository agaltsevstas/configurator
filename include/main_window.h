#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "scene.h"
#include "graphics_view.h"
#include "dialog_window.h"

#include <QMainWindow>

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
    MainWindow(std::map<std::string, std::string> &filePaths, std::map<std::string, bool> *types = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// Добавление кнопки на сцену
    void addButton();                    
    /// Сохранение всех связей между модулями в сообщение и переход к диалоговому окну.
    void on_generateYmlButton_clicked(); 
    /// Проверка на содержание хотя бы 1 связи между модулями.
    void timerAlarm();                   

private:
    std::map<std::string, std::string> _filePaths;
    Ui::MainWindow *_mainwindow = nullptr; /// Главное окно
    GraphicsView *_graphicsView = nullptr; /// Виджет
    DialogWindow *_dialogWindow = nullptr; /// Диалоговое окно
    Scene *_scene  = nullptr; /// Сцена
    QTimer *_timer = nullptr; /// Таймер
};

#endif // MAIN_WINDOW_H

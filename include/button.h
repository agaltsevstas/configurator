#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QMouseEvent>

/**
 * Класс Кнопка
 * Содержит название модуля и сигналы нажатий по кнопке.
 */
class Button : public QPushButton
{
    Q_OBJECT

public:
    /**
     * Конструктор
     * @param module Имя модуля.
     */
    Button(const QString &name, QWidget *parent = nullptr);
    inline const QString getName() const { return _name; } /// Получение названия.

signals:
    void rightClicked(); /// Нажатие правой кнопки на мыши.
    void leftClicked();  /// Нажатие левой кнопки на мыши (не задействованно).
    void midClicked();   /// Нажатие средней кнопки мыши (не задействованно).

protected:    
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString _name; /// Название.
};

#endif // BUTTON_H

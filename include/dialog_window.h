#ifndef DIALOG_WINDOW_H
#define DIALOG_WINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextStream>

namespace Ui
{
    class DialogWindow;
}

/**
 * Класс Диалоговое окно
 * Выводит текст сообщения.
 * Сохраняет сообщение.
 */
class DialogWindow : public QWidget
{
    Q_OBJECT

public:
    DialogWindow(QWidget *parent = 0);
    ~DialogWindow();
    void setText(QString &text) { text_ = text; } /// Получение сообщения.

signals:
    void mainWindow(); /// Открытие главного окна.

private slots:
    void on_cancelButton_clicked(); /// Возвращение к главному окну.
    void on_okButton_clicked();     /// Сохранение текста сообщения.
    void on_ymlButton_clicked();    /// Открытие / Закрытие окна с текстом сообщения.
    void on_lineEdit_textChanged(const QString &line); /// Ввод имени файла для сохранения.

private:
    const int width_  = 400; /// Ширина окна.
    const int height_ = 110; /// Высота окна.
    
    QString text_;     /// Текст сообщения.
    QString filePath_; /// Второе окно.
    
    Ui::DialogWindow  *dialogWindow_  = nullptr; /// Диалоговое окно.
    QPlainTextEdit    *plainTextEdit_ = nullptr; /// Окно с текстом сообщения.
};

#endif // DIALOG_WINDOW_H

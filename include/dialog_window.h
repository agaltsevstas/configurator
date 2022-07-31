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
    void setText(QString &text) { _text = text; } /// Получение сообщения.

signals:
    void mainWindow(); /// Открытие главного окна.

private slots:
    void on_cancelButton_clicked(); /// Возвращение к главному окну.
    void on_okButton_clicked();     /// Сохранение текста сообщения.
    void on_ymlButton_clicked();    /// Открытие / Закрытие окна с текстом сообщения.
    void on_lineEdit_textChanged(const QString &line); /// Ввод имени файла для сохранения.

private:
    const int _width = 400; /// Ширина окна.
    const int _height = 110; /// Высота окна.
    
    QString _text;     /// Текст сообщения.
    QString _filePath; /// Второе окно.
    
    Ui::DialogWindow  *_dialogWindow  = nullptr; /// Диалоговое окно.
    QPlainTextEdit *_plainTextEdit = nullptr;    /// Окно с текстом сообщения.
};

#endif // DIALOG_WINDOW_H

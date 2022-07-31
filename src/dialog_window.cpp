#include "dialog_window.h"
#include "ui_dialog_window.h"

#include <QFile>
#include <QMessageBox>

#include <boost/filesystem.hpp>

DialogWindow::DialogWindow(QWidget *parent) :
    QWidget (parent),
    _dialogWindow (new Ui::DialogWindow)
{    
    QString linetext_ = "structure.yml"; // Название сохраняемого файла
    _dialogWindow->setupUi(this);        // Установка настроек с формы ui
    _dialogWindow->lineEdit->setText(linetext_); // Установка исходного названия файла
    move(qApp->primaryScreen()->availableGeometry().center()); // Установка диалогового окна по центру экрана
}

DialogWindow::~DialogWindow()
{
    delete _dialogWindow;
}

void DialogWindow::on_cancelButton_clicked()
{
    bool isChecked = _dialogWindow->ymlButton->isCheckable();

    // Если окно с текстом сообщения открыто, закрываем
    if (isChecked)
    {
        _dialogWindow->ymlButton->setText("Открыть");
        _dialogWindow->ymlButton->setCheckable(false);
        _dialogWindow->gridLayout_2->removeWidget(_plainTextEdit);
        resize(_width, _height);
        resize(_width, _height);
        delete _plainTextEdit;
    }
    close();           // Закрытие окна
    emit mainWindow(); // Вызов главного окна
}

void DialogWindow::on_okButton_clicked()
{
    QString fileName = _dialogWindow->lineEdit->text(); // Имя файла
    QString path = _dialogWindow->comboBox->currentText(); // Путь
    _filePath = path + fileName;

    // Проверка на существование файла с таким же именем
    if (boost::filesystem::exists(_filePath.toStdString()))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Предупреждение"), tr("Файл уже существует, хотите перезаписать?"));
        if (reply == QMessageBox::Yes)
            QApplication::quit();
        else if (reply == QMessageBox::No)
            return;
    }
    // Проверка на существование папки
    if (!boost::filesystem::is_directory(path.toStdString()))
            boost::filesystem::create_directory(path.toStdString());

    bool isChecked = _dialogWindow->ymlButton->isCheckable(); // Проверка на открытие окна с текстом сообщения
    QFile file(_filePath);
    file.open(QFile::WriteOnly);
    QTextStream out(&file);

    if (!isChecked)
    {
        out << _text; // Вывод текста сообщения в файл
    }
    else
    {
        _text = _plainTextEdit->toPlainText(); // Вывод содержимого открытого окна в файл
        out << _text;
    }
    file.close();
    close();
}

void DialogWindow::on_lineEdit_textChanged(const QString &line)
{
    QString extension = line.right(4);

    // Проверка на расширение файла
    extension == ".yml" ? _dialogWindow->okButton->setEnabled(true): _dialogWindow->okButton->setEnabled(false);
}

void DialogWindow::on_ymlButton_clicked()
{
    bool isChecked = _dialogWindow->ymlButton->isCheckable();

    // Открытие окна с текстом сообщения
    if (!isChecked)
    {
        _dialogWindow->ymlButton->setText("Закрыть");
        _dialogWindow->ymlButton->setCheckable(true);
        _plainTextEdit = new QPlainTextEdit(this);
        // Установка параметров для окна
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setVerticalStretch(1);
        _plainTextEdit->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Courier 10 Pitch")); // Стиль шрифта
        font.setBold(true); // Жирный шрифт
        font.setPointSize(10); // Размер шрифта
        _plainTextEdit->setFont(font);
        _plainTextEdit->setPlainText(_text); // Добавление текста в окно
        _plainTextEdit->setCursorWidth(2);
        _plainTextEdit->setFrameShape(QFrame::WinPanel); // Рамка окна
        _plainTextEdit->setFrameShadow(QFrame::Plain);
        _plainTextEdit->setTabStopDistance(8); // Шаг для tab
        _dialogWindow->gridLayout_2->addWidget(_plainTextEdit);
        int addToHeight = std::count(_text.begin(), _text.end(), '\n'); // Подсчет кол-ва строк в сообщении
        resize(_width, _height + addToHeight * 20); // Установка размеров для окна
    }
    // Закрытие окна с текстом сообщения
    else
    {
        _dialogWindow->ymlButton->setText("Открыть");
        _dialogWindow->ymlButton->setCheckable(false);
        _text = _plainTextEdit->toPlainText();
        _dialogWindow->gridLayout_2->removeWidget(_plainTextEdit);
        resize(_width, _height);
        resize(_width, _height);
        delete _plainTextEdit;
    }
}

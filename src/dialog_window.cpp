#include <QMessageBox>
#include <QDesktopWidget>

#include <boost/filesystem.hpp>

#include "dialog_window.h"
#include "ui_dialog_window.h"

using namespace boost::filesystem;

DialogWindow::DialogWindow(QWidget *parent) :
    QWidget (parent),
    dialogWindow_ (new Ui::DialogWindow)
{    
    QString linetext_ = "structure.yml"; // Название сохраняемого файла
    dialogWindow_->setupUi(this);        // Установка настроек с формы ui
    dialogWindow_->lineEdit->setText(linetext_); // Установка исходного названия файла
    move(qApp->desktop()->availableGeometry(this).center()-rect().center()); // Установка диалогового окна по центру экрана
}

DialogWindow::~DialogWindow()
{
    delete dialogWindow_;
}

void DialogWindow::on_cancelButton_clicked()
{
    bool isChecked = dialogWindow_->ymlButton->isCheckable();

    // Если окно с текстом сообщения открыто, закрываем
    if (isChecked)
    {
        dialogWindow_->ymlButton->setText("Открыть");
        dialogWindow_->ymlButton->setCheckable(false);
        dialogWindow_->gridLayout_2->removeWidget(plainTextEdit_);
        resize(width_, height_);
        resize(width_, height_);
        delete plainTextEdit_;
    }
    close();           // Закрытие окна
    emit mainWindow(); // Вызов главного окна
}

void DialogWindow::on_okButton_clicked()
{
    QString fileName = dialogWindow_->lineEdit->text(); // Имя файла
    QString path = dialogWindow_->comboBox->currentText(); // Путь
    filePath_ = path + fileName;

    // Проверка на существование файла с таким же именем
    if (exists(filePath_.toStdString()))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Предупреждение"), tr("Файл уже существует, хотите перезаписать?"));
        if (reply == QMessageBox::Yes)
            QApplication::quit();
        else if (reply == QMessageBox::No)
            return;
    }
    // Проверка на существование папки
    if (!is_directory(path.toStdString()))
            create_directory(path.toStdString());

    bool isChecked = dialogWindow_->ymlButton->isCheckable(); // Проверка на открытие окна с текстом сообщения
    QFile file(filePath_);
    file.open(QFile::WriteOnly);
    QTextStream out(&file);

    if (!isChecked)
    {
        out << text_; // Вывод текста сообщения в файл
    }
    else
    {
        text_ = plainTextEdit_->toPlainText(); // Вывод содержимого открытого окна в файл
        out << text_;
    }
    file.close();
    close();
}

void DialogWindow::on_lineEdit_textChanged(const QString &line)
{
    QString extension = line.right(4);

    // Проверка на расширение файла
    extension == ".yml" ? dialogWindow_->okButton->setEnabled(true):dialogWindow_->okButton->setEnabled(false);
}

void DialogWindow::on_ymlButton_clicked()
{
    bool isChecked = dialogWindow_->ymlButton->isCheckable();

    // Открытие окна с текстом сообщения
    if (!isChecked)
    {
        dialogWindow_->ymlButton->setText("Закрыть");
        dialogWindow_->ymlButton->setCheckable(true);
        plainTextEdit_ = new QPlainTextEdit(this);
        // Установка параметров для окна
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setVerticalStretch(1);
        plainTextEdit_->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Courier 10 Pitch")); // Стиль шрифта
        font.setBold(true);    // Жирный шрифт
        font.setPointSize(10); // Размер шрифта
        plainTextEdit_->setFont(font);
        plainTextEdit_->setPlainText(text_); // Добавление текста в окно
        plainTextEdit_->setCursorWidth(2);
        plainTextEdit_->setFrameShape(QFrame::WinPanel); // Рамка окна
        plainTextEdit_->setFrameShadow(QFrame::Plain);
        plainTextEdit_->setTabStopWidth(8); // Шаг для tab
        dialogWindow_->gridLayout_2->addWidget(plainTextEdit_);
        int addToHeight = std::count(text_.begin(), text_.end(), "\n"); // Подсчет кол-ва строк в сообщении
        resize(width_, height_ + addToHeight * 20);                     // Установка размеров для окна
    }
    // Закрытие окна с текстом сообщения
    else
    {
        dialogWindow_->ymlButton->setText("Открыть");
        dialogWindow_->ymlButton->setCheckable(false);
        text_ = plainTextEdit_->toPlainText();
        dialogWindow_->gridLayout_2->removeWidget(plainTextEdit_);
        resize(width_, height_);
        resize(width_, height_);
        delete  plainTextEdit_;
    }
}

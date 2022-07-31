#include "button.h"
#include "configurator.h"
#include "item.h"
#include "main_window.h"
#include "ui_main_window.h"

#include <QTimer>

MainWindow::MainWindow(std::map<std::string, std::string> &filePaths, std::map<std::string, bool> *types, QWidget *parent) :
    QMainWindow   (parent),
    _filePaths    (filePaths),
    _mainwindow   (new Ui::MainWindow),
    _graphicsView (new GraphicsView(this)),
    _dialogWindow (new DialogWindow()),
    _scene        (new Scene()),
    _timer        (new QTimer(this))
{
    _mainwindow->setupUi(this);                        // Установка настроек с формы ui
    _graphicsView->setScene(_scene);                   // Добавление сцены в виджет
    _mainwindow->gridLayout->addWidget(_graphicsView); // Добавление виджета в главное окно
    _mainwindow->generateYmlButton->setEnabled(false); // Сделать кнопку недоступной
    _timer->start(100);                                // Таймер = 100 msec

    // Добавление кнопок в главное окно
    for (const auto &it: _filePaths)
    {
        Button *button = new Button(QString::fromStdString(it.first), this);
        // Проверка состояния каждого модуля
        if (types != nullptr && (*types)[it.first] == false)
            button->setEnabled(false);
        _mainwindow->verticalLayout->addWidget(button);
        connect(button, &Button::leftClicked, this, &MainWindow::addButton); // Отслеживание на нажатие кнопки
    }
    connect(_dialogWindow, &DialogWindow::mainWindow, this, &MainWindow::show); // Отслеживание перехода к диалоговому окну
    connect(_timer, SIGNAL(timeout()), this, SLOT(timerAlarm())); // Отслеживание таймера на наличие хотя бы 1 связи между модулями
    move(qApp->primaryScreen()->availableGeometry().center()); // Установка главного окна по центру экрана
}

MainWindow::~MainWindow()
{
    delete _mainwindow;
}

void MainWindow::addButton()
{
    Button *button = static_cast<Button*>(sender());    
    button->setEnabled(false);                       // Делаем кнопку недоступной, после добавления на сцену
    Item *item = new Item(button);
    QPointF newCoordinates = _scene->coordinates();  // Новые координаты для элемента на сцене
    item->setPos(newCoordinates);
    _scene->addItem(item);                           // Добавляем элемент на графическую сцену
}

void MainWindow::on_generateYmlButton_clicked()
{
    bool isChanged = _scene->getIsChanged();

    // Если была соединена / разорвана связь между модулями
    if (isChanged)
    {
        Configurator *configurator = new Configurator;
        // Добавление связи в конфигуратор
        for (const auto &link: _scene->getLinks())
        {
            std::string input = link.first.toStdString();
            std::string output = link.second.toStdString();
            const std::map<std::string, std::string> filePaths =
            {
                {input,  _filePaths[input]},
                {output, _filePaths[output]}
            };
            configurator->addTypes(input, output, filePaths);
        }
        // Вывод всех ожидаемых параметров модуля в текст сообщения
        const QString space(3, ' ');
        const QString yamlVersion = "%YAML:1.0";
        QString tab;
        QString text;
        QTextStream fs(&text);

        fs << yamlVersion << Qt::endl;
        fs << "---" << Qt::endl;
        fs << "modules:" << Qt::endl;
        auto types = configurator->getTypes();
        // Модули
        for (const auto &pair: types)
        {
            tab = space;
            fs << tab << "-" << Qt::endl;
            tab = space.repeated(2);
            fs << tab << "id: " << pair.first << Qt::endl;
            fs << tab << "type: " << QString::fromStdString(pair.second.name) << Qt::endl;

            std::vector<std::vector<std::string>> parametersVector = pair.second.parametersVector;
            if (!parametersVector.empty())
            {
                bool isFalseParam = false;
                bool isFalseParams = parametersVector[0][2] == "false" ? true : false;
                const QString comment = "#"; // Комментарий
                if (isFalseParams)
                    fs << comment;
                fs << tab << "parameters: " << Qt::endl;
                tab = space.repeated(3);
                for (std::vector<std::string> &parameters: parametersVector)
                {
                    if (parameters[2] == "false")
                    {
                        if (!isFalseParam)
                        {
                            fs << comment << tab << QString::fromUtf8("Список необязательных параметров") << Qt::endl;
                            isFalseParam = true;
                        }
                        fs << comment;
                    }
                    if (parameters[1] == "dtString")
                        parameters[3] = "\"" + parameters[3] + "\"";
                    fs << tab << QString::fromStdString(parameters[0]) << ": " << QString::fromStdString(parameters[3]) << Qt::endl;
                }
            }
        }
        fs << Qt::endl;
        // Вывод всех связей между модулями в текст сообщения
        std::map<uint, std::vector<uint>> links = configurator->getLinks();
        // Связи
        fs << "links: " << Qt::endl;
        for (const auto &pair: links)
        {
            uint from = pair.first;
            for (const uint to: pair.second)
            {
                tab = space;
                fs << tab << "-" << Qt::endl;
                tab = space.repeated(2);
                fs << tab << "from: " << from << Qt::endl;
                fs << tab << "to: " << to << Qt::endl;
            }
        }
        _dialogWindow->setText(text); // Отправка сообщения в диалоговое окно
        _scene->setIsChanged(false);
        delete configurator;
    }
    _dialogWindow->show();
    close();
}

void MainWindow::timerAlarm()
{
    // Если кнопка generateYmlButton недоступна
    if (_scene->getLinks().empty())
    {
        _mainwindow->generateYmlButton->setStyleSheet(QStringLiteral("background-color: white;")); // Стиль кнопки в отключенном состоянии
        _mainwindow->generateYmlButton->setEnabled(false);
    }
    else
    {
        // Стиль кнопки в включенном состоянии
        _mainwindow->generateYmlButton->setStyleSheet(QStringLiteral("color: white;" "background-color: #0000ff;"));
        _mainwindow->generateYmlButton->setEnabled(true); // Сделать кнопку доступной
    }
}

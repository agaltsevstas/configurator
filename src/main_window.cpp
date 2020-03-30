#include <QTimer>
#include <QDesktopWidget>

#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(map<string, string> &filePaths, map<string, bool> *types, QWidget *parent) :
    QMainWindow   (parent),
    filePaths_    (filePaths),
    mainwindow_   (new Ui::MainWindow),
    graphicsView_ (new GraphicsView(this)),
    dialogWindow_ (new DialogWindow()),
    scene_        (new Scene(this)),
    timer_        (new QTimer(this))
{    
    mainwindow_->setupUi(this);                        // Установка настроек с формы ui
    graphicsView_->setScene(scene_);                   // Добавление сцены в виджет
    mainwindow_->gridLayout->addWidget(graphicsView_); // Добавление виджета в главное окно
    mainwindow_->generateYmlButton->setEnabled(false); // Сделать кнопку недоступной
    timer_->start(100);                                // Таймер = 100 msec

    // Добавление кнопок в главное окно
    for (const auto &it: filePaths_)
    {
        Button *button = new Button(QString::fromStdString(it.first), this);
        // Проверка состояния каждого модуля
        if (types != nullptr && (*types)[it.first] == false)
            button->setEnabled(false);
        mainwindow_->verticalLayout->addWidget(button);
        connect(button, &Button::leftClicked, this, &MainWindow::addButton); // Отслеживание на нажатие кнопки
    }
    connect(dialogWindow_, &DialogWindow::mainWindow, this, &MainWindow::show); // Отслеживание перехода к диалоговому окну
    connect(timer_, SIGNAL(timeout()), this, SLOT(timerAlarm()));               // Отслеживание таймера на наличие хотя бы 1 связи между модулями
    move(qApp->desktop()->availableGeometry(this).center()-rect().center());    // Установка главного окна по центру экрана
}

MainWindow::~MainWindow()
{
    delete mainwindow_;
}

void MainWindow::addButton()
{
    Button *button = static_cast<Button*>(sender());    
    button->setEnabled(false);                       // Делаем кнопку недоступной, после добавления на сцену
    Item *item = new Item(button);
    QPointF newCoordinates = scene_->coordinates();  // Новые координаты для элемента на сцене
    item->setPos(newCoordinates);
    scene_->addItem(item);                           // Добавляем элемент на графическую сцену
}

void MainWindow::on_generateYmlButton_clicked()
{
    bool isChanged = scene_->getIsChanged();

    // Если была соединена / разорвана связь между модулями
    if (isChanged)
    {
        Configurator *configurator = new Configurator;
        // Добавление связи в конфигуратор
        for (const auto &link: scene_->getLinks())
        {
            string input = link.first.toStdString();
            string output = link.second.toStdString();
            const map<string, string> filePaths =
            {
                {input,  filePaths_[input]},
                {output, filePaths_[output]}
            };
            configurator->addTypes(input, output, filePaths);
        }
        // Вывод всех ожидаемых параметров модуля в текст сообщения
        const QString space(3, ' ');
        const QString yamlVersion = "%YAML:1.0";
        QString tab;
        QString text;
        QTextStream fs(&text);

        fs << yamlVersion << endl;
        fs << "---" << endl;
        fs << "modules:" << endl;
        auto types = configurator->getTypes();
        // Модули
        for (auto pair: types)
        {
            tab = space;
            fs << tab << "-" << endl;
            tab = space.repeated(2);
            fs << tab << "id: " << pair.first << endl;
            fs << tab << "type: " << QString::fromStdString(pair.second.name) << endl;

            vector<vector<string>> parametersVector = pair.second.parametersVector;
            if (!parametersVector.empty())
            {
                bool isFalseParam = false;
                bool isFalseParams = parametersVector[0][2] == "false" ? true : false;
                const QString comment = "#"; // Комментарий
                if (isFalseParams)
                    fs << comment;
                fs << tab << "parameters: " << endl;
                tab = space.repeated(3);
                for (vector<string> &parameters: parametersVector)
                {
                    if (parameters[2] == "false")
                    {
                        if (!isFalseParam)
                        {
                            fs << comment << tab << QString::fromUtf8("Список необязательных параметров") << endl;
                            isFalseParam = true;
                        }
                        fs << comment;
                    }
                    if (parameters[1] == "dtString")
                        parameters[3] = "\"" + parameters[3] + "\"";
                    fs << tab << QString::fromStdString(parameters[0]) << ": " << QString::fromStdString(parameters[3]) << endl;
                }
            }
        }
        fs << endl;
        // Вывод всех связей между модулями в текст сообщения
        map<uint, vector<uint>> links = configurator->getLinks();
        // Связи
        fs << "links: " << endl;
        for (const auto &pair: links)
        {
            uint from = pair.first;
            for (const uint to: pair.second)
            {
                tab = space;
                fs << tab << "-" << endl;
                tab = space.repeated(2);
                fs << tab << "from: " << from << endl;
                fs << tab << "to: " << to << endl;
            }
        }
        dialogWindow_->setText(text); // Отправка сообщения в диалоговое окно
        scene_->setIsChanged(false);
        delete configurator;
    }
    dialogWindow_->show();
    close();
}

void MainWindow::timerAlarm()
{
    QPalette pal = palette();

    // Если кнопка generateYmlButton недоступна
    if (scene_->getLinks().empty())
    {
        mainwindow_->generateYmlButton->setStyleSheet(QStringLiteral("background-color: white;"));  // Стиль кнопки в отключенном состоянии
        mainwindow_->generateYmlButton->setEnabled(false);
    }
    else
    {
        mainwindow_->generateYmlButton->setStyleSheet(QStringLiteral("color: white;"
                                                                     "background-color: #0000ff;")); // Стиль кнопки в включенном состоянии
        mainwindow_->generateYmlButton->setEnabled(true);                                            // Сделать кнопку доступной
    }
}

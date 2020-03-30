#include <QApplication>

#include <iostream>

#include "main_window.h"

/**
 * Карта исходных состояний модулей.
 */
map<string, bool> types =
{{"module_1",  false},
 {"module_2",  true },
 {"module_3",  false},
 {"module_4",  true },
 {"module_5",  false},
 {"module_6",  true },
 {"module_7",  true },
 {"module_8",  false},
 {"module_9",  true },
 {"module_10", true },
 {"module_11", true },
 {"module_12", true },
 {"module_13", true },
 {"module_14", true },
 {"module_15", true },
 {"module_16", true },
 {"module_17", true },
 {"module_18", true },
 {"module_19", true },
 {"main",      true }};

/**
 * Парсинг всех файлов modules.
 * Если у модуля есть заголовочный файл, совпадающий с именем его папки, то данный модуль добавляется
 * в Карту путей к модулям.
 */
map<string, string> getModules()
{
    bool isFound = false;                    // Нахождение модуля
    const string pathProject = "../modules"; // Относительный путь к проекту
    string module;                           // Название модуля
    map<string, string> filePaths;           // Карта путей к модулям
    recursive_directory_iterator iter(pathProject);
    recursive_directory_iterator end;
    try
    {
        while (iter != end)
        {
            //// Метод позволяющий после нахождения определенного условия остаться в текущем каталоге
//            iter.no_push();
            int levelPath = iter.level(); // Уровень папки относительно пути проекта
            string directory = iter->path().filename().c_str(); // Название папки

            // Проверка на папку
            if (is_directory(iter->path()))
            {
                if (levelPath == 0)
                {
                    module = directory;
                }
                else if (levelPath == 1 && directory == "include")
                {
                    // Требуемое условие, чтобы перейти к содержимому папки
                }
            }
            // Поиск заголовочного файла, совпадающего с названием папки
            else if (levelPath == 2 && module == basename(directory) &&
                    (extension(directory) == ".h" || extension(directory) == ".hpp"))
            {
                string filePath = pathProject + module + "/include/" + directory;
                filePaths[module] = filePath;
                iter.pop(); // Переход на уровень 1 относительно пути проекта
                iter.pop(); // Переход на уровень 0 относительно пути проекта
                isFound = true;
            }
            if (!isFound)
            {
                ++iter;
            }
            else
                isFound = false;
            //// Метод пытающийся перейти к имени следующего файла во вложенной последовательности
//            boost::system::error_code ec;
//            iter.increment(ec);
//            if (ec)
//            {
//                cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << endl;
//            }
        }
    }
    catch (system_error &se)
    {
        cerr << "Exception >> " << se.what() << endl;
    }
    return filePaths;
}

int main(int argc, char *argv[])
{
    map<string, string> filePaths = getModules();
    QApplication a(argc, argv);
    MainWindow w(filePaths, &types);
    w.show();

    return a.exec();
}

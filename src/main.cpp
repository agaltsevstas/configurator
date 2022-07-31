#include "main_window.h"

#include <QApplication>

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

using namespace boost::program_options;

/**
 * Карта исходных состояний модулей.
 */
std::map<std::string, bool> types =
{{"module_1",  true },
 {"module_2",  true },
 {"module_3",  true },
 {"module_4",  true },
 {"module_5",  true },
 {"module_6",  true },
 {"module_7",  true },
 {"module_8",  true },
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
 {"main",      false}};

/**
 * Парсинг всех файлов modules.
 * Если у модуля есть заголовочный файл, совпадающий с именем его папки, то данный модуль добавляется
 * в Карту путей к модулям.
 */
std::map<std::string, std::string> getModules(const std::string &directory)
{
    bool isFound = false; // Нахождение модуля
    std::string module;  // Название модуля
    std::map<std::string, std::string> directories; // Карта путей к модулям
    boost::filesystem::recursive_directory_iterator iter(directory);
    boost::filesystem::recursive_directory_iterator end;
    std::vector<std::string> names;
    try
    {
        while (iter != end)
        {
            //// Метод позволяющий после нахождения определенного условия остаться в текущем каталоге
//            iter.no_push();
            int levelPath = iter.level(); // Уровень папки относительно пути проекта
            std::string folder = iter->path().filename().c_str(); // Название папки

            // Проверка на папку
            if (is_directory(iter->path()))
            {
                if (levelPath == 0)
                {
                    module = folder;
                }
                else if (levelPath == 1 && folder == "include")
                {
                    // Требуемое условие, чтобы перейти к содержимому папки
                }
            }
            // Поиск заголовочного файла, совпадающего с названием папки
            else if (levelPath == 2 && module == boost::filesystem::basename(folder) &&
                    (boost::filesystem::extension(folder) == ".h" || boost::filesystem::extension(folder) == ".hpp"))
            {
                std::string filePath = directory + module + "/include/" + folder;
                directories[module] = filePath;
                names.push_back(module);
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
        }
    }
    catch (std::exception &ex)
    {
        std::cerr << "Exception >> " << ex.what() << std::endl;
    }
    return directories;
}

int main(int argc, char *argv[])
{
    std::string directory = "../modules/"; // Путь к модулям
    if (argc > 1)
    {
        std::string parameter;
        options_description desc("Требуемые опции");
        desc.add_options()
                ("help,h", "Помощь")
                ("directory,d", value<std::string>(&parameter), "Путь к модулям");
        try
        {
            variables_map vm;
            store(parse_command_line(argc, argv, desc), vm);
            notify(vm);

            if (vm.count("help"))
            {
                std::cout << desc << std::endl;
                return 0;
            }

            if (vm.count("directory"))
            {
                directory = vm["directory"].as<std::string>();
                std::cout << "Выбран путь к модулям >> " << directory << std::endl;
            }
            else
            {
                throw std::string(argv[1]);
            }
        }
        catch(const std::string &exception)
        {
            std::cerr << "Неверный параметр >> " << exception << std::endl;
        }
        catch(const std::exception &ex)
        {
            std::cerr << "Неверный параметр >> " << ex.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Неизвестная ошибка!" << std::endl;
        }
    }
    replace(directory.begin(), directory.end(), '\\', '/');
    if (*directory.rbegin() != '/')
    {
        directory += "/";
    }
    std::map<std::string, std::string> filePaths = getModules(directory);
    QApplication a(argc, argv);
    MainWindow w(filePaths, &types);
    w.show();

    return a.exec();
}

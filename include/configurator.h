#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <fstream>
#include <map>
#include <vector>

/**
 * Конфигурационный класс
 * Содержит используемые модули и связи между ними.
 */
class Configurator
{
private:

    std::map<std::string, bool> _bools =
    {
        {"true",  true  },
        {"false", false }
    };

    /**
     * Внутренняя структура одного из модулей.
     */
    struct Type
    {
        std::string name; /// Название модуля.
        /// Список ожидаемых (обязательных и необязательных) параметров модуля.
        std::vector<std::vector<std::string>> parametersVector;
    };

public:
    Configurator() {}    
    /// Получение карты взаимодействующих модулей.
    inline const std::map<uint, Type> getTypes() const { return _types; }
    /// Получение карты связей между модулями.
    inline const std::map<uint, std::vector<uint>> getLinks() const { return _links; }
    /// Добавление двух взаимодействующих модулей  и путей к ним.
    void addTypes(const std::string &input, const std::string &ouput, const std::map<std::string, std::string> &filePaths);

private:
    int  findType(const std::string &name); /// Поиск модуля среди ранее добавленных модулей.
    void addType(const std::string &name); /// Добавление модуля.
    std::vector<std::vector<std::string>> parsingFile(const std::string &fileName); /// Парсинг заголовочного файла модуля для поиска списка ожидаемых параметров.

    uint _id = 0; /// Идентификатор модуля.
    std::map<uint, Type> _types; /// Карта взаимодействующих модулей.
    std::map<std::string, std::string> _filePaths; /// Карта путей к модулям.
    std::map<uint, std::vector<uint>> _links; /// Карта связей между модулями.
};
#endif // CONFIGURATOR_H

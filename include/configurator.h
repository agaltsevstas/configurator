#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <fstream>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>

using namespace std;
using namespace boost::filesystem;

/**
 * Конфигурационный класс
 * Содержит используемые модули и связи между ними.
 */
class Configurator
{
private:

    map<string, bool> bools_ =
    {
        {"true",  true  },
        {"false", false }
    };

    /**
     * Внутренняя структура одного из модулей.
     */
    struct Type
    {
        string name; /// Название модуля.
        /// Список ожидаемых (обязательных и необязательных) параметров модуля.
        vector<vector<string>> parametersVector; 
    };

public:
    Configurator() {}    
    /// Получение карты взаимодействующих модулей.
    inline const map<uint, Type> getTypes() const { return types_; }        
    /// Получение карты связей между модулями.
    inline const map<uint, vector<uint>> getLinks() const { return links_; }
    /// Добавление двух взаимодействующих модулей  и путей к ним.
    void addTypes(const string &input, const string &ouput, const map<string, string> &filePaths);

private:    
    uint id_ = 0; /// Идентификатор модуля.
    
    map<uint, Type>         types_;     /// Карта взаимодействующих модулей.
    map<string, string>     filePaths_; /// Карта путей к модулям.
    map<uint, vector<uint>> links_;     /// Карта связей между модулями.

    int  findType(const string &name);  /// Поиск модуля среди ранее добавленных модулей.
    void addType(const string &name);   /// Добавление модуля.
    vector<vector<string>> parsingFile(const string &fileName); /// Парсинг заголовочного файла модуля для поиска списка ожидаемых параметров.
};
#endif // CONFIGURATOR_H

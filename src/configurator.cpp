#include "configurator.h"

#include <iostream>
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>

inline std::string removeSymbols(const std::string &line, const std::string &symbols)
{
    std::string result(line);
    result.erase(boost::remove_if(result, boost::is_any_of(symbols)), result.end());
    return result;
}

void Configurator::addTypes(const std::string &input, const std::string &output, const std::map<std::string, std::string> &filePaths)
{
    int idInput  = findType(input);
    int idOutput = findType(output);
    bool isFoundInput  = idInput > 0 ? true : false;
    bool isFoundOutput = idOutput > 0 ? true : false;
    _filePaths = filePaths;

    // Если два модуля отсутсвуют
    if (!isFoundInput && !isFoundOutput)
    {
        addType(input);
        idInput = _id;
        addType(output);
        idOutput = _id;
        _links[idInput].push_back(idOutput);
    }
    // Если два модуля присутсвуют
    if (isFoundInput && isFoundOutput)
    {
        std::vector<uint> ids = {static_cast<uint>(idInput), static_cast<uint>(idOutput)};

        for (const uint id: ids)
        {
            for (const auto &link: _links[id])
            {
                if (link == static_cast<uint>(idInput) || link == static_cast<uint>(idOutput))
                    return;
            }
        }
        _links[idInput].push_back(idOutput);
    }
    // Если входящий модуль присутствует, а выходящий отсутствует
    else if (isFoundInput && !isFoundOutput)
    {
        addType(output);
        idOutput = _id;
        _links[idInput].push_back(idOutput);
    }
    // Если входящий модуль отсутствует, а выходящий присутствует
    else if (!isFoundInput && isFoundOutput)
    {
        addType(input);
        idInput = _id;
        _links[idInput].push_back(idOutput);
    }
    sort(_links[idInput].begin(), _links[idInput].end());
}

int Configurator::findType(const std::string &name)
{
    int id = -1;
    for (const auto &pair: _types)
    {
        if (pair.second.name == name)
        {
            id = pair.first;
            return id;
        }
    }
    return id;
}

void Configurator::addType(const std::string &name)
{
    ++_id;
    Type type;
    type.name = name;
    type.parametersVector = parsingFile(_filePaths[name]);
    _types[_id] = type;
}

std::vector<std::vector<std::string>> Configurator::parsingFile(const std::string &fileName)
{
    const std::string expectedStart = "EXPECTED_START";
    const std::string expectedEnd   = "EXPECTED_END";
    const std::string parameter     = "PARAMETER";
    std::string line;
    std::vector<std::string> parameters;
    std::vector<std::vector<std::string>> parametersVector;
    std::ifstream file;

    file.open(fileName, std::ios::in);
    try
    {
        while (getline(file, line))
        {
            line = removeSymbols(line, " \t");
            if (line == expectedStart)
            {
                do
                {
                    getline(file, line);
                    line = removeSymbols(line, " ()\"");
                    if (line.empty() || line[0] == '/')
                    {
                        continue;
                    }
                    if (line == expectedEnd)
                    {
                        break;
                    }
                    line.erase(line.end() - 1); // Удаление в конце ;
                    std::string lineCopy = line.substr(parameter.size(), line.size());
                    boost::split(parameters, lineCopy, boost::is_any_of(","));
                    if (parameters.size() > 4)
                    {
                        while (parameters.size() > 4)
                        {
                            auto it = parameters.begin() + 4;
                            parameters[3] += "," + *it;
                            parameters.erase(it);
                        }
                    }
                    if (parameters.size() != 4)
                    {
                        throw std::to_string(parameters.size());
                    }
                    else if (parameters[3] == "true") // Изменение параметра true на 1
                    {
                        parameters[3] = "1";
                    }
                    else if (parameters[3] == "false") // Изменение параметра false на 0
                    {
                        parameters[3] = "0";
                    }
                    parametersVector.push_back(parameters);
                } // Считывать строку пока есть слово PARAMETER, или строка empty, или строка комментарий
                while (line.substr(0, parameter.size()) == parameter || line.empty() || line[0] == '/');
                if (line == expectedEnd)
                {
                    break;
                }
                else
                {
                    parametersVector.clear(); // Если отсутсвует слово EXPECTED_END
                }
            }
        }
    }
    catch(const std::string &exception)
    {
        std::cerr << "Недостаточное кол-во параметров EXPECTED >> " << exception << std::endl;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Ошибка >> " << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Неизвестная ошибка!" << std::endl;
    }
    // Сортировка по обязательным параметрам
    std::stable_sort(parametersVector.begin(), parametersVector.end(), [this]
                (const auto &first, const auto &second)
    {
        return _bools[first[2]] > _bools[second[2]];
    });

    return parametersVector;
}

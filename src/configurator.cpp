#include <iostream>
#include <utility>

#include "configurator.h"

inline string removeSymbols(const string &line, const string &symbols)
{
    string result(line);
    result.erase(boost::remove_if(result, boost::is_any_of(symbols)), result.end());
    return result;
}

void Configurator::addTypes(const string &input, const string &output, const map<string, string> &filePaths)
{
    int idInput  = findType(input);
    int idOutput = findType(output);
    bool isFoundInput  = idInput > 0 ? true : false;
    bool isFoundOutput = idOutput > 0 ? true : false;
    filePaths_ = filePaths;

    // Если два модуля отсутсвуют
    if (!isFoundInput && !isFoundOutput)
    {
        addType(input);
        idInput = id_;
        addType(output);
        idOutput = id_;
        links_[idInput].push_back(idOutput);
    }
    // Если два модуля присутсвуют
    if (isFoundInput && isFoundOutput)
    {
        vector<uint> ids = {static_cast<uint>(idInput), static_cast<uint>(idOutput)};

        for (const uint id: ids)
        {
            for (const auto &link: links_[id])
            {
                if (link == static_cast<uint>(idInput) || link == static_cast<uint>(idOutput))
                    return;
            }
        }
        links_[idInput].push_back(idOutput);
    }
    // Если входящий модуль присутствует, а выходящий отсутствует
    else if (isFoundInput && !isFoundOutput)
    {
        addType(output);
        idOutput = id_;
        links_[idInput].push_back(idOutput);
    }
    // Если входящий модуль отсутствует, а выходящий присутствует
    else if (!isFoundInput && isFoundOutput)
    {
        addType(input);
        idInput = id_;
        links_[idInput].push_back(idOutput);
    }
    sort(links_[idInput].begin(), links_[idInput].end());
}

int Configurator::findType(const string &name)
{
    int id = -1;
    for (const auto &pair: types_)
    {
        if (pair.second.name == name)
        {
            id = pair.first;
            return id;
        }
    }
    return id;
}

void Configurator::addType(const string &name)
{
    ++id_;
    Type type;
    type.name = name;
    type.parametersVector = parsingFile(filePaths_[name]);
    types_[id_] = type;
}

vector<vector<string>> Configurator::parsingFile(const string &fileName)
{
    const string expectedStart = "EXPECTED_START";
    const string expectedEnd   = "EXPECTED_END";
    const string parameter     = "PARAMETER";
    string line;
    vector<string> parameters;
    vector<vector<string>> parametersVector;
    std::ifstream file;

    file.open(fileName, ios::in);
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
                    string lineCopy = line.substr(parameter.size(), line.size());
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
                        throw to_string(parameters.size());
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
    catch(const string &exception)
    {
        cerr << "Недостаточное кол-во параметров EXPECTED >> " << exception << endl;
    }
    catch(const exception &ex)
    {
        cerr << "Ошибка >> " << ex.what() << endl;
    }
    catch(...)
    {
        cerr << "Неизвестная ошибка!" << endl;
    }
    // Сортировка по обязательным параметрам
    stable_sort(parametersVector.begin(), parametersVector.end(), [this]
                (const auto &first, const auto &second)
    {
        return bools_[first[2]] > bools_[second[2]];
    });

    return parametersVector;
}

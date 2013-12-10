#ifndef _BUILDLIST_H_
#define _BUILDLIST_H_

#include <ifstream>

class BuildList
{
private:
    std::string              _filename;
    std::vector<std::string> _nameList;

    void readBuildList(std::string filename)
    {
        std::ifstream file;

        file.open(filename);

        if (!file.good())
        {
            std::throw std::invalid_argument("BuildList: wrong filename " + filename);
        }

        while (file.good())
        {
            std::string currentLine;
            file >> currentLine;
            _nameList.push_back(currentLine);
        }

        file.close();
    }

    std::vector<std::string>::iterator _currentItem;

public:
    BuildList(std::string filename)
        :_filename(filename)
        ,_currentItem(_nameList.begin())
    {
        readBuildList(filename);
    }

    std::string current()
    {
        return *_currentItem;
    }

    State advance()
    {
        State returnState = State::InProgress;

        _currentItem++;
        if (_currentItem == _nameList.end())
        {
            returnState = State::Finished;
        }

        return returnState;
    }

    enum class State
    {
        InProgress,
        Finished
    };
};

#endif

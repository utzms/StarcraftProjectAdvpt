#ifndef _BUILDLIST_H_
#define _BUILDLIST_H_

#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <algorithm>


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
            throw std::invalid_argument("BuildList: wrong filename " + filename);
        }

        while (file.good())
        {

			std::string currentLine;
			std::getline(file, currentLine);
			currentLine.erase(std::remove(currentLine.begin(), currentLine.end(), ' '), currentLine.end());

			if (!currentLine.empty())
			{
				_nameList.push_back(currentLine);
			}
        }

		_currentItem = _nameList.begin();
        file.close();
    }

    std::vector<std::string>::iterator _currentItem;

public:
    enum class State
    {
        InProgress,
        Finished
    };

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

	void reset()
	{
		_currentItem = _nameList.begin();
	}

	std::vector<std::string> getAsVector()
	{
		return _nameList;
	}
};

#endif

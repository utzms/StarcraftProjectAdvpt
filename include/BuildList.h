#ifndef _BUILDLIST_H_
#define _BUILDLIST_H_

#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

class BuildList
{
private:
        std::string              _filename;
        std::vector<std::string> _nameList;
        std::vector<bool>		 _builtList;


        std::vector<std::string>::iterator _currentItem;
        std::vector<bool>::iterator		   _currentItemOk;

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
            // shouldn't builtList be initialized here with false completely?
            _builtList.assign(_nameList.size(), false);
            _currentItemOk = _builtList.begin();
        }

        BuildList(std::vector<std::string> nameList)
            :_nameList(nameList)
            ,_currentItem(_nameList.begin())
        {
            _builtList.assign(_nameList.size(), false);
            _currentItemOk = _builtList.begin();
        }

        ~BuildList()
        {
            _nameList.clear();
            _builtList.clear();
        }
        std::vector<std::string> getNameList()
        {
            return _nameList;

        }

        std::string current()
        {
            return *_currentItem;
        }

        State advance()
        {
            State returnState = State::InProgress;

            _currentItem++;
            _currentItemOk++;

            if (_currentItem == _nameList.end())
            {
                returnState = State::Finished;
            }

            return returnState;
        }

        void reset()
        {
            _currentItem = _nameList.begin();
            _currentItemOk = _builtList.begin();
        }

        std::vector<std::string> getAsVector()
        {
            return _nameList;
        }

        bool isCurrentItemOk()
        {
            return *_currentItemOk;
        }

        void setCurrentItemOk()
        {
            *_currentItemOk = true;
        }

        bool allItemsOk()
        {
            bool result = true;

            for (auto item : _builtList)
            {
                if (item == false)
                {
                    result = false;
                }
            }

            return result;
        }
};

#endif

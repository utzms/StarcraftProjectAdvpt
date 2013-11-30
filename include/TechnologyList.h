#ifndef _TECHNOLOGYLIST_H_
#define _TECHNOLOGYLIST_H_

#include <iostream>
#include <map>
#include <string>
#include <boost/tokenizer.hpp>
#include <sstream>

#include <Technology.h>
#include <DataReader.h>

class TechnologyList
{
	private:
		std::map<std::string,Technology*> units;

	public:
		TechnologyList();

		void initUnitList(std::string race);

		Technology *findUnit(std::string key);
};

#endif

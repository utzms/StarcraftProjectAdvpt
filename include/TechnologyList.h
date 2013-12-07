#ifndef _TECHNOLOGYLIST_H_
#define _TECHNOLOGYLIST_H_

#include <iostream>
#include <map>
#include <string>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <memory>

#include "Technology.h"
#include "DataReader.h"


class TechnologyList
{
	private:
		std::multimap<std::string, std::shared_ptr<Technology>> unresolvedBuildingRequirements;
		std::multimap<std::string, std::shared_ptr<Technology>> unresolvedUnitRequirements;

        std::multimap<std::string, std::shared_ptr<Technology>> units;
        std::multimap<std::string, std::shared_ptr<Technology>> buildings;

	public:
		TechnologyList();

		void initUnitList(std::string race);
		void initBuildingList(std::string race);

		std::shared_ptr<Technology> findUnit(std::string key);
		std::shared_ptr<Technology> findBuilding(std::string key);
};

#endif

#ifndef _TECHNOLOGYLIST_H_
#define _TECHNOLOGYLIST_H_

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <memory>

#include "Technology.h"
#include "DataReader.h"


class TechnologyList
{
	struct requirementRest
	{
		std::string req;
		std::string vanish;
		std::string buildFrom;
	};
	private:
		bool initialized;

		void initRest();

		std::map<std::shared_ptr<Technology>, requirementRest> unresolvedBuildingRequirements;
		std::map<std::shared_ptr<Technology>, requirementRest> unresolvedUnitRequirements;
		std::string _buildingPath;
		std::string _unitPath;

		std::multimap<std::string, std::shared_ptr<Technology> > units;
		std::multimap<std::string, std::shared_ptr<Technology> > buildings;

	public:
		TechnologyList(std::string buildingPath, std::string unitPath);

		inline std::string getBuildingPath() {return _buildingPath;}
		inline std::string getUnitPath() {return _unitPath;}

		bool isInitialized();
		void initUnitList(std::string race);
		void initBuildingList(std::string race);

		std::vector<std::shared_ptr<Technology> > findUnitVec(std::string key);
		std::vector<std::shared_ptr<Technology> > findBuildingVec(std::string key);

		std::shared_ptr<Technology> findUnit(std::string key);
		std::shared_ptr<Technology> findBuilding(std::string key);
};

#endif

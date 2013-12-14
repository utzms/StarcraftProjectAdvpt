#ifndef _TECHNOLOGYLIST_H_
#define _TECHNOLOGYLIST_H_

#include <iostream>
#include <stdexcept>
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
		std::string buildingPath, unitPath;
		std::vector<std::vector<std::shared_ptr<Technology>>> passedRequirements;

		void initRest();
		void reset();

		std::map<std::shared_ptr<Technology>, requirementRest> unresolvedBuildingRequirements;
		std::map<std::shared_ptr<Technology>, requirementRest> unresolvedUnitRequirements;

		std::multimap<std::string, std::shared_ptr<Technology> > units;
		std::multimap<std::string, std::shared_ptr<Technology> > buildings;

	public:
		TechnologyList(std::string buildingPath, std::string unitPath);
		TechnologyList();

		inline std::string getBuildingPath() {return buildingPath;}
		inline std::string getUnitPath() {return unitPath;}
		void setBuildingPath(std::string buildPath) {this->buildingPath=buildPath;}
		void setUnitPath(std::string unitPath) {this->unitPath=unitPath;}

		bool checkRequirements(std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>>);
		bool isBuildListPossible(std::vector<std::string> wuff);

		bool isInitialized();
		void initUnitList(std::string race);
		void initBuildingList(std::string race);
		void printAll();

		std::vector<std::vector<std::shared_ptr<Technology>>> getPassedRequirements();

		std::vector<std::shared_ptr<Technology>> findUnitVec(std::string key);
		std::vector<std::shared_ptr<Technology>> findBuildingVec(std::string key);

		std::shared_ptr<Technology> findUnit(std::string key);
		std::shared_ptr<Technology> findBuilding(std::string key);
};

#endif

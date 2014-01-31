#ifndef _TECHNOLOGYLIST_H_
#define _TECHNOLOGYLIST_H_

#include <iostream>
#include <stdexcept>
#include <map>
#include <forward_list>
#include <string>
#include <sstream>
#include <memory>
#include <random>
#include <set>
#include <chrono>

#include "Debug.h"
#include "Technology.h"
#include "DataReader.h"
#include "RacePolicy.h"


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
		//needed global variables for the random Engine
		std::minstd_rand0 randomEngine;
		std::uniform_int_distribution<int> uniformDist;
		std::vector<std::string> techNames;
        std::set<std::string> techSet;
		std::string buildingPath, unitPath;

        void initRest();

		//std::map<std::shared_ptr<Technology>, requirementRest> unresolvedBuildingRequirements;
		//std::map<std::shared_ptr<Technology>, requirementRest> unresolvedUnitRequirements;
		std::forward_list<std::pair<std::shared_ptr<Technology>, requirementRest>> unresolvedBuildingRequirements;
		std::forward_list<std::pair<std::shared_ptr<Technology>, requirementRest>> unresolvedUnitRequirements;

		std::multimap<std::string, std::shared_ptr<Technology> > units;
        std::multimap<std::string, std::shared_ptr<Technology> > buildings;


    public:
		TechnologyList(std::string buildingPath, std::string unitPath);
		TechnologyList();
		TechnologyList(const TechnologyList &);
		TechnologyList(TechnologyList &);
		~TechnologyList();

        void initRandomGenerator(size_t seed, std::string SpecialOne="", int weight=0);
		std::string getRandomTechnology();

		void linkRequirement(std::shared_ptr<Technology> &tech, std::vector<std::string> in, RequirementType T);
		void linkRequirement(std::shared_ptr<Technology> &tech, std::vector<std::vector<std::string>> in, RequirementType T);
		void tokenizeString(std::vector<std::string> &out, std::string in, char sep);
		std::shared_ptr<Technology> findTechnology(std::string);

		template <typename T>
		inline bool streamToVariable(std::stringstream &stream, T &x);
		void cleanUnresolved();
		void cleanAll();
		void reset();
		inline std::string getBuildingPath() {return buildingPath;}
		inline std::string getUnitPath() {return unitPath;}
		void setBuildingPath(std::string buildPath) {this->buildingPath=buildPath;}
		void setUnitPath(std::string unitPathIn) {this->unitPath=unitPathIn;}

		bool isInitialized();
		void initUnitList(std::string race);
		void initBuildingList(std::string race);
		void printAll();

		std::vector<std::vector<std::shared_ptr<Technology>>> getPassedRequirements();

		std::vector<std::shared_ptr<Technology>> findUnitVec(std::string key);
		std::vector<std::shared_ptr<Technology>> findBuildingVec(std::string key);

		std::shared_ptr<Technology> findUnit(std::string key);
		std::shared_ptr<Technology> findBuilding(std::string key);

        void initTechnologySet();
        std::set<std::string> getTechnologySet(void);

		TechnologyList & operator= (const TechnologyList & other) 
		{
			PROGRESS("COPY ASSIGNMENT");
			if (&other != this)
			{
				this->units = other.units;
				this->buildings = other.buildings;
				this->initialized = other.initialized;
				this->unitPath = other.unitPath;
				this->buildingPath = other.buildingPath;
				this->unresolvedBuildingRequirements = other.unresolvedBuildingRequirements;        
                this->techSet = other.techSet;

                /* no longer required
                for (auto &it : this->units)
				{
					it.second->setExistence(0);
				}
				for (auto &it : this->buildings)
				{
					it.second->setExistence(0);
                }
                */
			}
			return *this;
        }


};

#endif

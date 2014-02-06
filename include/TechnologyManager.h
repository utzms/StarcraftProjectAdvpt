#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <climits>

#include "GameState.h"
#include "TechnologyList.h"
#include "Technology.h"
#include "InitTechTree.hpp"
#include "Debug.h"


/** Manager class responsible for ensuring that all requirements are fulfilled before an action can be executed.
 *
 * The TechnologyManager is aware of the whole Tech Tree of the three races
 * and of the amount of resources that have to be spent for creating a specific Entity.
 * If a new Entity shall be created, request() can be called and the TechnologyManager checks if everything
 * concerning Resources and Tech in the GameState fits the requirements for its creation.
 * The TechnologyManager is notified whenever a change concerning the Tech Tree occurs.
 */
template <typename RacePolicy>
class TechnologyManager : private RacePolicy 
{

private:
    std::shared_ptr<GameState> mGameState;
    TechnologyList mTechList;
    std::map<std::string,int> mExistenceMap;

    inline void createExistenceEntries(void)
    {
        const auto& techSet = mTechList.getTechnologySet();
        if(techSet.size() <= 0)
        {
            throw std::logic_error("@TechnologyManager::createExistenceEntries: There are currently no entries to add!");
        }
        for(std::string name : techSet)
        {
            /* currently not supported in the g++ version installed in the Huber-CIP
            mExistenceMap.emplace(name,0);
            */
            mExistenceMap.insert(std::pair<std::string,int>(name,0));
        }
    }

    inline void initializeExistence(void)
    {
        if(mExistenceMap.size() <= 0)
        {
            throw std::logic_error("@TechnologyManager::initializeExistence: Empty existenceMap, nothing to initialize");
        }
        const auto& initBuildings = mTechList.findBuildingVec(RacePolicy::getMainBuilding());
        const auto& initUnits = mTechList.findUnitVec(RacePolicy::getWorker());
        const auto& initSpecials = mTechList.findUnitVec(RacePolicy::getSpecialResource());
        for (const auto special : initSpecials)
        {
            mExistenceMap[special->getName()] = 100000; //With INT_MAX we risc integer overflows! 
        }
        for (const auto building : initBuildings)
        {
            mExistenceMap[building->getName()] = 1;
        }
        for (const auto unit : initUnits)
        {
            mExistenceMap[unit->getName()] = 6;
        }
    }

    inline void constructExistenceMap()
    {
        this->createExistenceEntries();
        this->initializeExistence();
    }

    inline void resetExistence(void)
    {
        for(auto it = mExistenceMap.begin(); it != mExistenceMap.end(); ++it)
        {
            it->second = 0;
        }
    }

    inline bool checkCosts(std::shared_ptr<Technology> technology)
    {
        if(mGameState == false || mGameState.use_count() == 0)
        {
                throw std::logic_error("TechnologyManager::checkCosts: There is no internal GameState, Costs can not be checked here");
        }
        if(technology->getMineralsCost() > mGameState->getMinerals() ||
                technology->getGasCost() > mGameState->getGas() )
        {
            return false;
        }
        else if(!this->checkIfNameIsBuilding(technology->getName()) && technology->getSupplyCost() > mGameState->getAvailableSupply())
        {
            return false;
        }

        return true;
    }

    inline bool checkRequirements(std::shared_ptr<Technology> technology)
    {
        std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType> > > requirements = technology->getRequirements();
        bool fulfilled = false;
        for(auto redundantRequirements : requirements)
        {
            fulfilled = true;
            std::map<std::string, int> vanishMap;
            for(auto requirement : redundantRequirements)
            {
                std::map<std::string, int>::iterator it;
                if (requirement.second == RequirementType::Vanishing)
                {
                    std::string name = (requirement.first)->getName();
                    if (entityExists(name))
                    {
                        fulfilled=true;
                        if ((it=vanishMap.find(name)) == vanishMap.end())
                        {
                            std::pair<std::string, int> input(name,1);
                            vanishMap.insert(input);
                        } else
                        {
                            ++((*it).second);
                        }
                    } else
                    {
                        fulfilled=false;
                        break;
                    }
                }
            }
            if(!fulfilled) return false;
            for (auto it : vanishMap)
            {
                if (it.second > this->getExistenceCountForName(it.first))
                {
                //	std::cout << "\t    missing:" << it.first << std::endl;
                    return false;
                }
            }
            fulfilled = true;
            for(auto requirement : redundantRequirements)
            {
                if (requirement.second != RequirementType::Vanishing)
                {
                    fulfilled=false;
                    if (this->entityExists((requirement.first)->getName()))
                    {
                        fulfilled=true;
                        break;
                    }
                }
            }
            if(!fulfilled) return false;
        }
        return true;
    }

    inline bool checkEverything(std::shared_ptr<Technology> technology)
    {
        if(this->checkCosts(technology))
        {
            return this->checkRequirements(technology);
        }
        return false;
    }

    inline void initTechnologyList(void)
    {
        if(!InitTechTree<RacePolicy>(&(this->mTechList)).initTechTree())
        {
            throw std::runtime_error("@TechnologyManager::initTechnologyListTechnologyList: initTechTree failed, something went terribly wrong!");
        }
        this->mTechList.initTechnologySet();
    }

public:

    //Standard Constructor
    TechnologyManager()
    {
        initTechnologyList();

        this->constructExistenceMap();

        PROGRESS("TechnologyManager: Standard Constructor");
    }

    TechnologyManager(std::shared_ptr<GameState> initialGameState)
        :mGameState(initialGameState)
    {
        //PROGRESS("TM Constructor");
        if(!this->mGameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        this->initTechnologyList();
        this->constructExistenceMap();

        PROGRESS("TechnologyManager: Constructor with GameState as argument");
    }

    TechnologyManager(std::shared_ptr<GameState> initialGameState, const TechnologyList& techList)
        :mGameState(initialGameState)
    {
        if(!this->mGameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        this->mTechList = techList;
        this->constructExistenceMap();

        PROGRESS("TechnologyManager: Constructor with TechnologyList and GameState as argument");

    }

    //only for BuildListGenerator designed
    TechnologyManager(TechnologyList &techList)
    {
        this->mTechList = techList;
        this->constructExistenceMap();

        PROGRESS("TechnologyManager: Constructor with Technology List as argument");
    }

    //only for BuildListGenerator designed
    TechnologyManager(const TechnologyList &techList)
    {
        this->mTechList = techList;
        this->constructExistenceMap();

        PROGRESS("TechnologyManager: Constructor with Technology List as argument");
    }


    // Copy-Constructor


    TechnologyManager(TechnologyManager<RacePolicy>& techManager)
    {
        this->mTechList = techManager.mTechList;
        this->mExistenceMap = techManager.mExistenceMap;
        this->resetExistence();
        this->initializeExistence();

        PROGRESS("TechnologyManager: Copy Constructor");
    }

    TechnologyManager(const TechnologyManager<RacePolicy>& techManager)
    {
        this->mTechList = techManager.mTechList;
        this->mExistenceMap = techManager.mExistenceMap;
        this->resetExistence();
        this->initializeExistence();

        PROGRESS("TechnologyManager: Copy Constructor");
    }



    ~TechnologyManager(){PROGRESS("TM Destructor");}

    std::vector< std::shared_ptr<Technology> > getTechnologyForName(std::string entityName)
    {
        const auto& technology = mTechList.findBuilding(entityName);
        if(technology == nullptr)
        {
            return mTechList.findUnitVec(entityName);
        }
        else
        {
            return mTechList.findBuildingVec(entityName);
        }
    }

    const TechnologyList& getTechnologyList()
    {
        return mTechList;
    }

    void checkEverythingAndGetVanishing(std::string entityName, std::pair<bool, std::vector<std::string> >& res)
    {
        std::vector<std::shared_ptr<Technology>> techVec = this->getTechnologyForName(entityName);
        if (techVec.size() == 0)
        {
            throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
        }
        for (const auto& tech : techVec)
        {
            if (this->checkEverything(tech))
            {
                res.first = true;
                std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > requirements = tech->getRequirements();
                for (const auto& redundantRequirements : requirements)
                {
                    for (const auto& requirement : redundantRequirements)
                    {
                        if (requirement.second == RequirementType::Vanishing)
                        {
                            (res.second).push_back(requirement.first->getName());
                        }
                    }
                }
                return;
            }
        }
        res.first = false;
    }

    void checkRequirementsAndGetVanishing(std::shared_ptr<Technology> tech, std::pair<bool, std::vector<std::string> >& res)
	{
		//std::cout << "\t" << tech->getName() << std::endl;
        if (checkRequirements(tech))
		{
			res.first = true;
			std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > requirements = tech->getRequirements();
			for (const auto& redundantRequirements : requirements)
			{
				for (const auto& requirement : redundantRequirements)
				{
					if (requirement.second == RequirementType::Vanishing)
					{
						//std::cout << "\t\t\t" << requirement.first->getName() << std::endl;
						(res.second).push_back(requirement.first->getName());
					}
				}
			}
			return;
		}
		res.first = false;
	}

	/*
	   void checkAndGetVanishing(std::vector<std::shared_ptr<Technology> > techVec, std::pair<bool, std::vector<std::string> >& res)
	   {
	   if (techVec.size() == 0)
	   {
	   throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
	   }
	   bool fulfilled=false;
	   std::vector<std::string> killed;
	   for (const auto& tech : techVec)
	   {
	   std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > requirements = tech->getRequirements();
	   for (const auto& redundantRequirements : requirements)
	   {
	   fulfilled=false;
	   for (const auto& requirement : redundantRequirements)
	   {
	   std::string name = requirement.first->getName();
	//for vanishing requirements all have to be existent and valid
	//otherwise its enough if one requirement is fulfilled
	if (requirement.second == RequirementType::Vanishing)
	{
	if (entityExists(name))
	{
	killed.push_back(name);
	notifyDestruction(name);
	fulfilled = true;
	} else
	{   
	fulfilled = false;
	break;
	}
	} else if (entityExists(name))
	{
	fulfilled = true;
	break;
	}
	}
	if (!fulfilled)
	{
	for (auto it : killed)
	{
	notifyCreation(it);
	}
	killed.clear();
	break;
	}
	}
	if (fulfilled==true)
	{
	break;
	}
	}
	res.first = fulfilled;
	for (auto it : killed)
	{   
	(res.second).push_back(it);
	notifyCreation(it);
	}   
	killed.clear();
	}
	 */

    std::vector<std::string> getRequirementsForName(std::string entityName)
	{
        std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::getTechnologyForName(entityName);
		if (techVec.size() == 0)
		{
			throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
		}
		std::vector<std::string> res(techVec.size()*10);
		for(auto tech : techVec)
		{
			std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > requirements = tech->getRequirements();
			for (auto redundantRequirements : requirements)
			{
				for (auto requirement : redundantRequirements)
				{
					res.push_back(requirement.first->getName());
				}
			}
		}
		return res;
	}

	bool isBuildListPossible(std::vector<std::string> buildList)
	{
		resetExistence();
		initializeExistence();
		bool fulfilled = false;
		float supply = 4;
		for(std::string entityName : buildList)
		{
            std::vector<std::shared_ptr<Technology>> techVec = this->getTechnologyForName(entityName);
			if(techVec.empty())
			{
				return false;
			}
			fulfilled = false;
			for(std::shared_ptr<Technology> tech : techVec)
			{
				if(checkIfNameIsBuilding(entityName) || tech->getSupplyCost() <= supply)
				{
					std::pair<bool,std::vector<std::string>> vanishPair;
                    this->checkRequirementsAndGetVanishing(tech, vanishPair);

					if(vanishPair.first == true)
					{
						fulfilled = true;
						for(std::string name : vanishPair.second)
						{
                            this->notifyDestruction(name);
						}
                        this->notifyCreation(entityName);
						if(!entityName.compare(RacePolicy::getMainBuilding()))
						{
							supply += 10;
						}
						else if(!entityName.compare(RacePolicy::getSupplyProvider()))
						{
							supply += 8;
						} else
						{
							supply -= tech->getSupplyCost();
							if (supply < 0)
							{
								resetExistence();
								return false;
							}
						}
						break;
					}
				}
			}
			if(!fulfilled)
			{
				resetExistence();
				return false;
			}
		}
		resetExistence();
		return true;
	}

	/** Functions for notifying state-changes in entities.
	 * @param Shared Pointer to the Entity that has changed it state
	 */
	void notifyCreation(std::string entityName)
	{
		if(mExistenceMap.count(entityName) == 0)
		{
			throw std::invalid_argument("@TechnologyManager::notifiyCreation: "+entityName+" is not part of the tech tree.");
		}

		auto it = mExistenceMap.find(entityName);
		if(it->second < 0)
		{
			throw std::range_error("@TechnologyManager::notifyCreation: The internal existence count for "+entityName+" reached a negative value. Something went terribly wrong " + std::to_string(it->second));
		}

		++(it->second);
	}

	void notifyDestruction(std::string entityName)
	{
		if(mExistenceMap.count(entityName) == 0)
		{
			throw std::invalid_argument("@TechnologyManager::notifiyDestruction: "+entityName+" is not part of the tech tree.");
		}

		auto it = mExistenceMap.find(entityName);
		if(it->second <= 0)
		{
			throw std::range_error("@TechnologyManager::notifyDestruction: The internal existence count for "+entityName+" reached a negative value. Something went terribly wrong " + std::to_string(it->second));
		}
		--(it->second);

	}

	// Functions for deciding if a name (e.g. from buildList)
	// is a building or a unit
	bool checkIfNameIsBuilding(std::string entityName)
	{
		bool result = false;

		auto technology = mTechList.findBuilding(entityName);
		if(technology != nullptr)
		{
			result = true;
		}

		return result;
	}

	bool checkIfNameIsUnit(std::string entityName)
	{
		bool result = false;

		auto technology = mTechList.findUnit(entityName);
		if(technology != nullptr)
		{
			result = true;
		}

		return result;
	}

    Costs getCostsForName(std::string entityName)
	{
		std::shared_ptr<Technology> technology;

        if (this->checkIfNameIsBuilding(entityName))
		{
			technology = mTechList.findBuilding(entityName);
		}
        else if (this->checkIfNameIsUnit(entityName))
		{
			technology = mTechList.findUnit(entityName);
		}
		else
		{
			throw std::invalid_argument("TechnologyManager::getEntityCosts() entity does not exist in tech list.");
		}

		return technology->TechCosts;
	}

	std::vector<std::string> getBuildingsForUnitProduction(std::string unitName)
	{
		std::shared_ptr<Technology> technology;

		if (checkIfNameIsUnit(unitName))
		{
			technology = mTechList.findUnit(unitName);
		}
		else
		{
			throw std::invalid_argument("TechnologyManager::getBuildingForUnitProduction() entity is not a unit.");
		}

		auto requirements = technology->getRequirements();
		std::vector<std::string> buildings;

		for (auto outerIterator : requirements)
		{
			for (auto innerIterator : outerIterator)
			{
				if (innerIterator.second == RequirementType::ForProduction)
				{
					buildings.push_back(innerIterator.first->getName());
				}
			}
		}

		return buildings;
	}

    int getExistenceCountForName(std::string name)
	{
		if (mExistenceMap.count(name) == 0)
			return 0;
		return mExistenceMap[name];
	}

    bool entityExists(std::string name)
	{
        if(mExistenceMap.count(name) == 0)
		{
            throw std::invalid_argument("@TechnologyManager::entityExists: "+name+" is not part of the tech tree.");
		}

		auto it = mExistenceMap.find(name);
		if(it->second < 0)
		{
            throw std::range_error("@TechnologyManager::entityExists: The internal existence count for "+name+" reached a negative value. Something went terribly wrong");
		}
		return it->second > 0 ? true : false;

	}

};

#endif

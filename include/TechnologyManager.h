#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>

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
    std::shared_ptr<GameState> _gameState;
    TechnologyList _techList;
    
    inline std::vector< std::shared_ptr<Technology> > findTechnology(std::string entityName)
    {
        auto technology = _techList.findBuilding(entityName);
        if(technology == nullptr)
        {
            return _techList.findUnitVec(entityName);
        }
        else
        {
            return _techList.findBuildingVec(entityName);
        }
    }

	inline bool checkTechnologyRequirements(std::shared_ptr<Technology> technology)
    {
       
        std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType> > > requirements = technology->getRequirements();
        bool fulfilled = false;

		for(auto redundantRequirements : requirements)
        {
            fulfilled = false;
            for(auto requirement : redundantRequirements)
            {
				if((requirement.first)->exists())
                {
                    fulfilled = true;
                    break;
                }
            }
            if(!fulfilled) return false;
        }
        return true;
    }
    inline bool checkTechnologyCosts(std::shared_ptr<Technology> technology)
    {
        if(technology->getMineralsCost() > _gameState->getMinerals() ||
                technology->getGasCost() > _gameState->getGas() )
        {
            return false;
        }
        else if(!checkIfNameIsBuilding(technology->getName()) && technology->getSupplyCost() > _gameState->getAvailableSupply())
        {
            return false;
        }

        return true;
    }
    inline bool checkTechnology(std::shared_ptr<Technology> technology)
    {
        if(checkTechnologyCosts(technology))
        {
			return checkTechnologyRequirements(technology);
        }
        return false;
    }

public:
    TechnologyManager(std::shared_ptr<GameState> initialGameState)
        :_gameState(initialGameState)
    {
        PROGRESS("TM Constructor");
        if(!_gameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        if(!InitTechTree<RacePolicy>(&_techList).initTechTree())
        {
            throw std::runtime_error("TechnologyList initialization failed. Something went terribly wrong!");
        }

        _techList.reset();
 
    }

    TechnologyManager(std::shared_ptr<GameState> initialGameState, const TechnologyList& techList)
            :_gameState(initialGameState)
    {
        if(!_gameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        _techList = techList;
        _techList.reset();
    }

    TechnologyManager(const TechnologyList &techList)
    {
            _techList = techList;
            _gameState = std::make_shared<GameState>();
            _techList.reset();
    }


    // Copy-Constructor
    TechnologyManager(const TechnologyManager& techManager)
    {
            _techList = techManager._techList;
            _gameState = techManager._gameState;
            _techList.reset();

    }

    //sry jonas, testzweck only. No problem
    TechnologyManager()
    {
        PROGRESS("WRONG TM Constructor");
    }
    ~TechnologyManager(){PROGRESS("TM Destructor");}

	inline bool isZerg()
	{
		if (findTechnology("Larva").size() > 0)
			return true;
		return false;
	}


    /** Function for demanding a requirements check.
         * @param The Entity that shall be created
         * @return true, if all requirements are fulfilled
         * @return false, else
         */

    bool checkEntityRequirements(std::string entityName)
    {

        std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entityName);
        if(techVec.size() == 0)
        {
            return false;
        }
        for(auto tech : techVec)
        {
            PROGRESS("NEXT TECHNOLOGY WILL BE TESTED");
            if(TechnologyManager::checkTechnology(tech))
            {
                return true;
            }
        }
        return false;
    }

	void checkAndGetVanishing(std::string entityName, std::pair<bool, std::vector<std::string> >& res)
    {
        std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entityName);
        if (techVec.size() == 0)
        {
            throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
        }
        for (auto tech : techVec)
        {
            if (TechnologyManager::checkTechnology(tech))
            {
                res.first = true;
                std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > requirements = tech->getRequirements();
                for (auto redundantRequirements : requirements)
                {
                    for (auto requirement : redundantRequirements)
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
    
	void printAll()
	{
		_techList.printAll();
	}
    bool isBuildListPossible(std::vector<std::string> buildList)
    {	

        _techList.reset();
        auto initBuildings = _techList.findBuildingVec(RacePolicy::getMainBuilding());
        auto initUnits = _techList.findUnitVec(RacePolicy::getWorker());
		auto larvafix = _techList.findUnitVec("Larva");
        for (auto unit : larvafix)
        {
            unit->setExistence(300000);
        } 
        for (auto building : initBuildings)
        {
            building->setExistence(1);
        }
        for (auto unit : initUnits)
        {
            unit->setExistence(6);
        } 
        bool fulfilled = false;
        float supply = 4;
        for(std::string entityName : buildList)
        {
            std::vector<std::shared_ptr<Technology>> techVec = findTechnology(entityName);
            if(techVec.empty())
            {
                return false;
            }
            fulfilled = false;
			for(std::shared_ptr<Technology> tech : techVec)
			{
				if(checkIfNameIsBuilding(entityName) || tech->getSupplyCost() <= supply) 
				{
					// sorry for change, this is a bad overall design bug
					// we dont have any larvae before simulation but they are a vanishing requirement
					if(checkTechnologyRequirements(tech))
					{
						fulfilled = true;
						notifyCreation(entityName);
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
								_techList.reset();
								return false;
							}
						}
						break;
					}
				}
			}
			if(!fulfilled) 
			{
				_techList.reset();
				return false;
			}
		}
		_techList.reset();
		return true;
	}

	/** Functions for notifying state-changes in entities.
	 * @param Shared Pointer to the Entity that has changed it state
	 */
	void notifyCreation(std::string entityName)
	{
		std::vector<std::shared_ptr<Technology>> techVec = /*TechnologyManager::*/findTechnology(entityName);
		if(techVec.size() == 0)
		{
			throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
		}
		for(auto tech : techVec)
		{
			tech->incExistence();
		}
	}

	void notifyDestruction(std::string entityName)
	{
		std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entityName);
		if(techVec.size() == 0)
		{
			throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
		}
		for(auto tech : techVec)
		{
			tech->decExistence();
		}
	}

	// Functions for deciding if a name (e.g. from buildList)
	// is a building or a unit
	bool checkIfNameIsBuilding(std::string entityName)
	{
		bool result = false;

		auto technology = _techList.findBuilding(entityName);
		if(technology != nullptr)
		{
			result = true;
		}

		return result;
	}

	bool checkIfNameIsUnit(std::string entityName)
	{
		bool result = false;

		auto technology = _techList.findUnit(entityName);
		if(technology != nullptr)
		{
			result = true;
		}

		return result;
	}

	Costs getEntityCosts(std::string entityName)
	{
		std::shared_ptr<Technology> technology;

		if (checkIfNameIsBuilding(entityName))
		{
			technology = _techList.findBuilding(entityName);
		}
		else if (checkIfNameIsUnit(entityName))
		{
			technology = _techList.findUnit(entityName);
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
			technology = _techList.findUnit(unitName);
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

	bool buildingExists(std::string buildingName)
	{
		std::shared_ptr<Technology> technology;

		if (checkIfNameIsBuilding(buildingName))
		{
			technology = _techList.findBuilding(buildingName);
		}
		else
		{
			throw std::invalid_argument("TechnologyManager::getBuildingForUnitProduction() entity is not a unit.");
		}

		return technology->exists();
	}
};

#endif

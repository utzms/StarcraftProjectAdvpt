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
    std::shared_ptr<GameState> _gameState;
    TechnologyList _techList;
    std::map<std::string,int> existenceMap;

    inline void initializeExistence(void)
    {
        const auto& initBuildings = _techList.findBuildingVec(RacePolicy::getMainBuilding());
        const auto& initUnits = _techList.findUnitVec(RacePolicy::getWorker());
        const auto& specialResources = _techList.findUnitVec(RacePolicy::getSpecialResource());
        for (const auto res : specialResources)
        {
            existenceMap.find(res->getName())->second = INT_MAX;
        }
        for (const auto building : initBuildings)
        {
            existenceMap[building->getName()] = 1;
        }
        for (const auto unit : initUnits)
        {
            existenceMap[unit->getName()] = 6;
        }
    }

    inline void resetExistence(void)
    {
        for(auto it = existenceMap.begin(); it != existenceMap.end(); ++it)
        {
            it->second;
        }
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
        //PROGRESS("TM Constructor");
        if(!_gameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        if(!InitTechTree<RacePolicy>(&_techList).initTechTree())
        {
            throw std::runtime_error("TechnologyList initialization failed. Something went terribly wrong!");
        }
        auto techNames = _techList.getTechnologySet();
        for(auto name : techNames)
        {
            existenceMap.insert(std::pair<std::string,int>(name,0));

        }
        initializeExistence();

        PROGRESS("TechnologyManager: Constructor with GameState as argument");
    }

    TechnologyManager(std::shared_ptr<GameState> initialGameState, const TechnologyList& techList)
        :_gameState(initialGameState)
    {
        if(!_gameState)
        {
            throw std::invalid_argument("Can not pass nullptr as initial argument");
        }
        _techList = techList;
        auto techNames = _techList.getTechnologySet();
        for(std::string name : techNames)
        {

            std::cout << name << std::endl;
            existenceMap.insert(std::pair<std::string,int>(name,0));
        }
        initializeExistence();

        PROGRESS("TechnologyManager: Constructor with TechnologyList and GameState as argument");

    }

    //only for BuildListGenerator designed
    TechnologyManager(TechnologyList &techList)
    {
        _techList = techList;
        _gameState = std::make_shared<GameState>();
        auto techNames = _techList.getTechnologySet();
        for(std::string name : techNames)
        {
            existenceMap.insert(std::pair<std::string,int>(name,0));
        }
        initializeExistence();

        PROGRESS("TechnologyManager: Constructor with Technology List as argument");
    }

    //only for BuildListGenerator designed
    TechnologyManager(const TechnologyList &techList)
    {
        _techList = techList;
        _gameState = std::make_shared<GameState>();
        auto techNames = _techList.getTechnologySet();
        for(std::string name : techNames)
        {
            existenceMap.insert(std::pair<std::string,int>(name,0));
        }
        initializeExistence();

        PROGRESS("TechnologyManager: Constructor with Technology List as argument");
    }


    // Copy-Constructor


    TechnologyManager(TechnologyManager<RacePolicy>& techManager)
    {
        _techList = techManager._techList;
        _gameState = techManager._gameState;
        existenceMap = techManager.existenceMap;
        resetExistence();
        initializeExistence();

        PROGRESS("TechnologyManager: Copy Constructor");
    }

    TechnologyManager(const TechnologyManager<RacePolicy>& techManager)
    {
        _techList = techManager._techList;
        _gameState = techManager._gameState;
        existenceMap = techManager.existenceMap;
        resetExistence();
        initializeExistence();

        PROGRESS("TechnologyManager: Copy Constructor");
    }


    //Standard Constructor
    TechnologyManager()
    {
        if(!InitTechTree<RacePolicy>(&_techList).initTechTree())
        {
            throw std::runtime_error("TechnologyList initialization failed. Something went terribly wrong!");
        }
        _gameState = std::make_shared<GameState>();
        auto techNames = _techList.getTechnologySet();
        for(std::string name : techNames)
        {
            existenceMap[name] = 0;
        }
        initializeExistence();
        PROGRESS("TechnologyManager: Standard Constructor");
    }

    ~TechnologyManager(){/*PROGRESS("TM Destructor")*/;}

    inline bool checkTechnologyRequirements(std::shared_ptr<Technology> technology)
    {

        std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType> > > requirements = technology->getRequirements();
        bool fulfilled = false;

        for(auto redundantRequirements : requirements)
        {
            fulfilled = false;
            for(auto requirement : redundantRequirements)
            {
                if(entityExists((requirement.first)->getName()))
                {
                    fulfilled = true;
                    break;
                }
            }
            if(!fulfilled) return false;
        }
        return true;
    }
    inline std::vector< std::shared_ptr<Technology> > findTechnology(std::string entityName)
    {
        const auto& technology = _techList.findBuilding(entityName);
        if(technology == nullptr)
        {
            return _techList.findUnitVec(entityName);
        }
        else
        {
            return _techList.findBuildingVec(entityName);
        }
    }

    bool technologyExists(std::string name)
    {    
        return existenceMap.count(name) > 0 ? true : false;
    }

    inline bool isZerg()
    {
        if (RacePolicy::getSpecialResource == "Zerg")
            return true;
        return false;
    }

    const TechnologyList& getTechnologyList()
    {
        return _techList;
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
            //PROGRESS("NEXT TECHNOLOGY WILL BE TESTED");
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
        for (const auto& tech : techVec)
        {
            if (TechnologyManager::checkTechnology(tech))
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

    std::vector<std::string> getEntityRequirements(std::string entityName)
    {
        std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entityName);
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

    void printAll()
    {
        _techList.printAll();
    }
    bool isBuildListPossible(std::vector<std::string> buildList)
    {
        resetExistence();
        initializeExistence();
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
        if(existenceMap.count(entityName) == 0)
        {
            throw std::invalid_argument("@TechnologyManager::notifiyCreation: "+entityName+" is not part of the tech tree.");
        }

        auto it = existenceMap.find(entityName);
        if(it->second < 0)
        {
            throw std::range_error("@TechnologyManager::notifyCreation: The internal existence count for "+entityName+" reached a negative value. Something went terribly wrong");
        }

        ++(it->second);
    }

    void notifyDestruction(std::string entityName)
    {
        if(existenceMap.count(entityName) == 0)
        {
            throw std::invalid_argument("@TechnologyManager::notifiyCreation: "+entityName+" is not part of the tech tree.");
        }

        auto it = existenceMap.find(entityName);
        if(it->second <= 0)
        {
            throw std::range_error("@TechnologyManager::notifyDestruction: The internal existence count for "+entityName+" reached a negative value. Something went terribly wrong");
        }
        --(it->second);

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

    bool entityExists(std::string name)
    {

        if(existenceMap.count(name) == 0)
        {
            throw std::invalid_argument("@TechnologyManager::entityExists: "+name+" is not part of the tech tree.");
        }

        auto it = existenceMap.find(name);
        if(it->second < 0)
        {
            throw std::range_error("@TechnologyManager::entityExists: The internal existence count for "+name+" reached a negative value. Something went terribly wrong");
        }
        return it->second > 0 ? true : false;

    }

    bool buildingExists(std::string buildingName)
    {
        return entityExists(buildingName);
    }

    std::string getRandomTechnologyName()
    {
            return _techList.getRandomTechnology();
    }

    std::map<std::string,int> getExistenceMap()
    {
        return existenceMap;
    }


};

#endif

#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include <memory>
#include <vector>
#include <utility>
#include <stdexcept>

#include "GameState.h"
#include "TechnologyList.h"
#include "Technology.h"
#include "InitTechTree.hpp"
#include "Unit.h"
#include "Worker.h"
#include "Building.h"


/** Manager class responsible for ensuring that all requirements are fulfilled before an action can be executed.
 * 
 * The TechnologyManager is aware of the whole Tech Tree of the three races 
 * and of the amount of resources that have to be spent for creating a specific Entity.
 * If a new Entity shall be created, request() can be called and the TechnologyManager checks if everything
 * concerning Resources and Tech in the GameState fits the requirements for its creation.
 * The TechnologyManager is notified whenever a change concerning the Tech Tree occurs.
 */

// innermost pair of Requirements: Technology, RequirementFlag(Vanishing, Existing, Creation means building)
// vector of innermost pairs: one of the pairs needs to be ok, each of those could be ok for the technology to be able to be built
// outer vector: every element needs to be fulfilled
//typedef std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementType> > > Requirements;

// a unit can be constructed from several similar technologies, so we need another vector
//typedef std::vector<std::pair<bool, std::pair<std::shared_ptr<Technology>, Requirements> > > RequirementsVec;

class TechnologyManager
{
	private:
        std::shared_ptr<GameState> _gameState;
        std::shared_ptr<TechnologyList> _techList;


        //inline bool getNeededRequirements(std::shared_ptr<Technology> tech, Requirements& res);
//        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Unit> unit)
//        {
//            return _techList->findUnitVec(unit->getName());
//        }

//        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Building> building)
//        {
//            return _techList->findBuildingVec(building->getName());
//        }

//        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Worker> worker)
//        {
//            return _techList->findUnitVec(worker->getName());
//        }

	public:
        TechnologyManager(std::shared_ptr<GameState> initialGameState, std::shared_ptr<TechnologyList> techList)
            :_gameState(initialGameState)
            ,_techList(techList)
        {
            if(!_gameState || !_techList)
            {
                throw std::invalid_argument("Can not pass nullptr as initial argument");
            }

			if(!InitTechTree(_techList).initTechTree())
            {
                throw std::runtime_error("TechnologyList initialization failed. Something went terribly wrong!");
            }
        }

    bool checkTechnologyRequirements(std::shared_ptr<Technology> technology)
        {
//		if(technology->getMineralsCost() > _gameState->getMinerals() || technology->getGasCost() > _gameState->getGas() || technology->getSupplyCost() > _gameState->getSupply())
//			{
//				return false;
//			}

//		 std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType> > > requirements = technology->getRequirements();
//            bool fulfilled = false;

//            for(auto redundantRequirements : requirements)
//            {
//                fulfilled = false;
//                for(auto requirement : redundantRequirements)
//                {
//                    if((requirement.first)->exists())
//                    {
//                        fulfilled = true;
//                        break;
//                    }
//                }
//                if(!fulfilled) return false;
//            }
            return true;
        }


        /** Function for demanding a requirements check.
         * @param The Entity that shall be created
         * @return true, if all requirements are fulfilled
         * @return false, else
         */

        // template argument deduction does the work here
        // we do not have to specify the template type argument
        // when calling this function
        template <class EntityType> bool checkEntityRequirements(std::shared_ptr<EntityType> entity)
        {
            // TODO change argument to std::string and adapt the implementation

//			std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entity);
//            if(techVec.size() == 0)
//            {
//                throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
//            }
//            for(auto tech : techVec)
//            {
//				if(TechnologyManager::checkTechnologyRequirements(tech))
//                {
//                    return true;
//                }
//            }
//            return false;
        }
/*
        template <class EntityType> std::shared_ptr<RequirementsVec> requestEntityRequirements(std::shared_ptr<EntityType> entity)
        {
//            // TODO implementation, and adaption to std::string as argument
//			std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entity);
//            if(techVec.size() == 0)
//            {
//                throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
//            }
//            auto res = std::shared_ptr<RequirementsVec>(new RequirementsVec(techVec.size()));
//            for(size_t i = 0; i != techVec.size(); ++i)
//            {
//                //TODO
//            }
//            return res;
        }
*/
	/** Functions for notifying state-changes in entities.
	 * @param Shared Pointer to the Entity that has changed it state
	 */
        template <class EntityType> void notifyCreation(std::shared_ptr<EntityType> entity)
        {
//			std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entity);
//            if(techVec.size() == 0)
//            {
//                throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
//            }
//            for(auto tech : techVec)
//            {
//                tech->setExistence(true);
//            }
        }

        template <class EntityType> void notifyDestruction(std::shared_ptr<EntityType> entity)
        {
//			std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager::findTechnology(entity);
//            if(techVec.size() == 0)
//            {
//                throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
//            }
//            for(auto tech : techVec)
//            {
//                tech->setExistence(false);
//            }
        }
};

#endif

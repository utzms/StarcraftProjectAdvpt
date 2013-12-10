#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include <memory>
#include <vector>
#include <utility>

#include "GameState.h"
#include "TechnologyList.h"
#include "Technology.h"
#include "InitTechTree.hpp"

/** Manager class responsible for ensuring that all requirements are fulfilled before an action can be executed.
 * 
 * The TechnologyManager is aware of the whole Tech Tree of the three races 
 * and of the amount of resources that have to be spent for creating a specific Entity.
 * If a new Entity shall be created, request() can be called and the TechnologyManager checks if everything
 * concerning Resources and Tech in the GameState fits the requirements for its creation.
 * The TechnologyManager is notified whenever a change concerning the Tech Tree occurs.
 */


typedef std::vector<std::vector<std::pair<std::shared_ptr<Technology>, RequirementFlag>>> Requirements;
typedef std::vector<std::pair<bool, std::pair<std::shared_ptr<Technology>, Requirements>>> RequirementsVec;

template <class Race> class TechnologyManager
{
	private:
        std::shared_ptr<GameState> gameState;
        std::shared_ptr<TechnologyList> techList;

        inline bool check(std::shared_ptr<Technology> requirement);
        inline bool getNeededRequirements(std::shared_ptr<Technology> tech, Requirements& res);
        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Unit> unit);
        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Building> building);
        inline std::vector< std::shared_ptr<Technology> > findTechnology(std::shared_ptr<Worker> worker);

	public:
        TechnologyManager(std::shared_ptr<GameState> initialGameState, std::shared_ptr<TechnologyList> techListPtr); 
        /** Function for demanding a requirements check.
         * @param The Entity that shall be created
         * @return true, if all requirements are fulfilled
         * @return false, else
         */

        // template argument deduction does the work here
        // we do not have to specify the template type argument
        // when calling this function
        template <class EntityType> bool checkRequirements(std::shared_ptr<EntityType> entity);

        template <class EntityType> std::shared_ptr<RequirementsVec> requestRequirements(std::shared_ptr<EntityType> entity); 
	/** Functions for notifying state-changes in entities.
	 * @param Shared Pointer to the Entity that has changed it state
	 */
        template <class EntityType> void notifyCreation(std::shared_ptr<EntityType> entity);
        template <class EntityType> void notifyDestruction(std::shared_ptr<EntityType> entity);
};

#endif

#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include "GameState.h"
#include "Entity.h"

/** Manager class responsible for ensuring that all requirements are fulfilled before an action can be executed.
 * 
 * The TechnologyManager is aware of the whole tech tree of the three races 
 * and of the amount of resources that have to be spent for creating a specific Entity.
 * If a new Entity shall be created, request() can be called and the TechnologyManager checks if everything
 * concerning Resources and Tech in the GameState fits the requirements for its creation.
 * The TechnologyManager is notified whenever a change that concerns the Tech Tree occurs.
 */

class TechnologyManager
{
	private:
		GameState * gameState;
	
	public:
        /** Function for demanding a requirement check.
         * @param The Entity that shall be created
         * @return true, if all requirements are fulfilled
         * @return false, else
         */
		bool request(Entity entity);
};

#endif

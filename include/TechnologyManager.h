#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_
#include <memory>
#include "GameState.h"
#include "Entity.h"
using std::shared_pointer;
using std::string;

/** Manager class responsible for ensuring that all requirements are fulfilled before an action can be executed.
 * 
 * The TechnologyManager is aware of the whole Tech Tree of the three races 
 * and of the amount of resources that have to be spent for creating a specific Entity.
 * If a new Entity shall be created, request() can be called and the TechnologyManager checks if everything
 * concerning Resources and Tech in the GameState fits the requirements for its creation.
 * The TechnologyManager is notified whenever a change concerning the Tech Tree occurs.
 */


class TechnologyManager
{
	private:
        shared_ptr<GameState> gameState;
	     
	public:
        TechnologyManager(string race, GameState *gameState); 
        /** Function for demanding a requirements check.
         * @param The Entity that shall be created
         * @return true, if all requirements are fulfilled
         * @return false, else
         */

        // template argument deduction does the work here
        // we do not have to specify the template type argument
        // when calling this function
        template <class T>
        bool request(const T& entity);

	/** Function for notifying state-changes in entities.
	 * @param The Entity that has changed it state
	 */
        template <class T>
        void notify(const T& entity);
};

#endif

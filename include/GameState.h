#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>
#include <memory>

#include "Worker.h"
#include "Building.h"
#include "Unit.h"

class Worker;

/** Class that represents the complete state of the game, enclosing all existing entities and the current amount of resources. */
class GameState
{
	private:	
		float minerals;
		float gas;
		float energy;
		float supply;

	public:
        std::vector< std::shared_ptr<Worker> >      workerList;
        std::vector< std::shared_ptr<Building> >    buildingList;
        std::vector< std::shared_ptr<Unit> >        unitList;

        float getMinerals()
        {
            return minerals;
        }

        float getGas()
        {
            return gas;
        }

        float getEnergy()
        {
            return energy;
        }

        float getSupply()
        {
            return supply;
        }

        void addMinerals(float value)
        {
            minerals += value;
        }

        void subMinerals(float value)
        {
            minerals -= value;
        }

        void addGas(float value)
        {
            gas += value;
        }

        void subGas(float value)
        {
            gas -= value;
        }

        void addEnergy(float value)
        {
            energy += value;
        }

        void subEnergy(float value)
        {
            energy -= value;
        }

        void addSupply(float value)
        {
            supply += value;
        }

        void subSupply(float value)
        {
            supply -= value;
        }
};

#endif // _GAMESTATE_H_

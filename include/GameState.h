#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>
#include <memory>

#include "Worker.h"
#include "Building.h"
#include "Unit.h"

/** Class that represents the complete state of the game, enclosing all existing entities and the current amount of resources. */
class GameState
{
	private:	
		float _minerals;
		float _gas;
		float _energy;
		float _supply;

	public:
		std::vector< std::shared_ptr<Worker> >      workerList;
		std::vector< std::shared_ptr<Building> >    buildingList;
		std::vector< std::shared_ptr<Unit> >        unitList;

		GameState()
		{
			_minerals	=	0.0f;
			_gas		=	0.0f;
			_energy		=	0.0f;
			_supply		=	0.0f;
		}

        float getMinerals()
        {
			return _minerals;
        }

        float getGas()
        {
			return _gas;
        }

        float getEnergy()
        {
			return _energy;
        }

        float getSupply()
        {
			return _supply;
        }

        void addMinerals(float value)
        {
			_minerals += value;
        }

        void subMinerals(float value)
        {
			_minerals -= value;
        }

        void addGas(float value)
        {
			_gas += value;
        }

        void subGas(float value)
        {
			_gas -= value;
        }

        void addEnergy(float value)
        {
			_energy += value;
        }

        void subEnergy(float value)
        {
			_energy -= value;
        }

        void addSupply(float value)
        {
			_supply += value;
        }

        void subSupply(float value)
        {
			_supply -= value;
        }
};

#endif // _GAMESTATE_H_

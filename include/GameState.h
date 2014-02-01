#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>
#include <iostream>
#include <memory>

#include "Worker.h"
#include "Building.h"
#include "Unit.h"

/** Class that represents the complete state of the game, enclosing all existing entities and the current amount of resources. */
class GameState
{
	private:	
        float mMinerals;
        float mGas;
		float _energy;
        int mSupplyCount;
        int mSupplyMax;

	public:
		std::vector< std::shared_ptr<Worker> >      workerList;
		std::vector< std::shared_ptr<Building> >    buildingList;
		std::vector< std::shared_ptr<Unit> >        unitList;

		GameState()
		{
            mMinerals	=	0.0f;
            mGas		=	0.0f;
			_energy		=	0.0f;
            mSupplyCount=	0.;
            mSupplyMax  =	0;
		}

        float getMinerals()
        {
            return mMinerals;
        }

        float getGas()
        {
            return mGas;
        }

        float getEnergy()
        {
			return _energy;
        }
		
        int getMaxSupply()
        {
            return mSupplyMax;
        }

        int getSupply()
        {
            return mSupplyCount;
        }
        int getAvailableSupply()
		{
            return (mSupplyMax-mSupplyCount);
		}

        void addMinerals(float value)
        {
            mMinerals += value;
        }

        void subMinerals(float value)
        {
            mMinerals -= value;
        }

        void addGas(float value)
        {
            mGas += value;
        }

        void subGas(float value)
        {
            mGas -= value;
        }

        void addEnergy(float value)
        {
			_energy += value;
        }

        void subEnergy(float value)
        {
			_energy -= value;
        }

        void addSupplyMax(int value)
		{
            mSupplyMax += value;
		}

        void addSupply(int value)
        {
            mSupplyCount += value;
        }

        void subSupply(int value)
        {
            mSupplyCount -= value;
        }

        void setEnergy(float value)
        {
            _energy = value;
        }

};

#endif // _GAMESTATE_H_

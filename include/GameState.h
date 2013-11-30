#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>

#include <Entity.h>
#include <Worker.h>
#include <Building.h>
#include <Unit.h>

class Worker;

/** Class that represents the complete state of the game, enclosing all existing Entities and the current amount of resources. */
class GameState
{
	private:	
		int minerals;
		int gas;
		int energy;
		int supply;
		std::vector<Worker> workerList;
		std::vector<Building> buildingList;
		std::vector<Unit> unitList;
//		std::vector<Worker&> workerList;	//compiler meckert
//		std::vector<Building&> buildingList;
//		std::vector<Unit&> unitList;
	public:
		int getMinerals(){return minerals;};
		int getGas(){return gas;};
		int getEnergy(){return energy;};
		int getSupply(){return supply;};

		void addMinerals(int value){minerals += value;};
		void subMinerals(int value){minerals -= value;};
		void addGas(int value){gas += value;};
        void subGas(int value){gas -= value;};
		void addEnergy(int value){energy += value;};
        void subEnergy(int value){energy -= value;};
		void addSupply(int value){supply += value;};
        void subSupply(int value){supply -= value;};
};

#endif

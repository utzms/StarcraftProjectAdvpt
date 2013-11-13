#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>

#include "Entity.h"
#include "Worker.h"
#include "Building.h"
#include "Unit.h"

class GameState
{
	private:
		//evtl diese hier als float.....aber eigentlich solln wirs als int modellieren
		int Minerals;
		int Gas;
		int Energy;
		int Supply;
		std::vector<Worker&> workerList;
		std::vector<Building&> buildingList;
		std::vector<Unit&> unitList;
	public:
		int getMinerals(){return Minerals;};
		int getGas(){return Gas;}:
		int getEnergy(){return Energy;};
		int getSupply(){return Supply;};

		void addMinerals(int count){Minerals += count;};
		void subMinerals(int count){Minerals -= count;};
		void addGas(int count){Gas += count;};
                void subGas(int count){Gas -= count;};
		void addEnergy(int count){Energy += count;};
                void subEnergy(int count){Energy -= count;};
		void addSupply(int count){Supply += count;};
                void subSupply(int count){Supply -= count;};

};

#endif

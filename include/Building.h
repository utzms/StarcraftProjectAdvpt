#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Debug.h"

class Building
{
	public:
		enum class ProductionType
		{
			WorkerOrder,
			UnitOrder
		};

	private:
		std::string _name;


	public:
        enum class State
        {
            UnderConstruction,
            Ready,
            Producing
        };

		enum class UpgradeState
		{
			Ready,
			Upgrading
		};

		State state;
		int	timer;




		std::string productionUnitName;
		ProductionType productionType;

		UpgradeState upgradeState;
		int upgradeTimer;
		std::string targetUpgradeName;

		Building(std::string name, int buildTime)
			:_name(name)
			,state(State::UnderConstruction)
			,timer(buildTime)
			,upgradeState(UpgradeState::Ready)
			,upgradeTimer(0)
		{
		}

        void timeStep()
        {
			if (state != State::Ready)
			{
				timer--;
			}

			if (upgradeState != UpgradeState::Ready)
			{
				upgradeTimer--;
			}
        }

        int getTime()
        {
			return timer;
        }

		std::string getName()
		{
			return _name;
		}
};

#endif

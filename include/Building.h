#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Debug.h"

class Building
{
	private:
		std::string _name;

	public:
        enum class ProductionType
        {
            WorkerOrder,
            UnitOrder
        };

        enum class State
        {
            UnderConstruction,
            Ready,
            Producing,
			ProducingDouble
        };

		enum class UpgradeState
		{
			Ready,
			Upgrading
		};

        enum class BoostState
        {
            NotBoosted,
            Boosted
        };

		State state;
        UpgradeState upgradeState;
        BoostState boostState;

        int	timer;
        int upgradeTimer;
        int boostTimer;

		std::string productionUnitName;
        std::string targetUpgradeName;

		ProductionType productionType;

		Building(std::string name, int buildTime)
			:_name(name)
			,state(State::UnderConstruction)
			,timer(buildTime)
			,upgradeState(UpgradeState::Ready)
			,upgradeTimer(0)
            ,boostState(BoostState::NotBoosted)
            ,boostTimer(0)
		{
		}

        void timeStep()
        {

            int decreasingValue = 1;
            if(boostState == BoostState::Boosted)
            {
                boostTimer--;
                decreasingValue = 2;
            }

			if (state != State::Ready)
			{
                timer -= decreasingValue;
                if(timer < 0)
                {
                    timer = 0;
                }
            }

			if (upgradeState != UpgradeState::Ready)
			{
                upgradeTimer -= decreasingValue;
                if(upgradeTimer < 0)
                {
                    upgradeTimer = 0;
                }
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

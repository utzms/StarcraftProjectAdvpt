#ifndef ENERGIZER_H
#define ENERGIZER_H

#include "GameState.h"
#include "TechnologyManager.h"

template
<
        class RacePolicy
>
class Energizer
{
    private:
        std::shared_ptr<GameState>  _gameState;
        std::shared_ptr<TechnologyManager<RacePolicy>> _technologyManager;
        float                       _energy;
        int                         _timer;
        RaceType                    _race;

        bool    _energyEntityExists;
        float   _energyLimit;

        int  _muleTimer;
        int  _queenTimer;

        int _abilityCooldown;
        std::vector<std::shared_ptr<Building>> boostedBuildingList;
    public:
        Energizer(std::shared_ptr<GameState> gameState, std::shared_ptr<TechnologyManager<RacePolicy>> technologyManager)
            :_gameState(gameState)
            ,_technologyManager(technologyManager)
            ,_energy(0)
            ,_timer(0)
            ,_race(RacePolicy::getRace())
            ,_energyEntityExists(false)
            ,_energyLimit(0)
            ,_muleTimer(0)
            ,_queenTimer(0)
            ,_abilityCooldown(0)

        {

        }

        void update()
        {
            if (!_energyEntityExists)
            {
                if (_race == RaceType::Protoss)
                {
                    _energyLimit = 100.f;

                    if (_energy < _energyLimit)
                    {
                        _energy += 0.5625f;
                    }

                    _energyEntityExists = true;
                }
                else if (_race == RaceType::Terran)
                {
                    for (auto buildingIterator : _gameState->buildingList)
                    {
                        if ((buildingIterator->getName().compare("OrbitalCommand") == 0))
                        {
							// Orbital Command starts out with 50 energy
							_energy += 50.f;

                            _energyLimit = 200.f;
                            if (_energy < _energyLimit)
                            {
                                _energy += 0.5625f;
                            }
                            _energyEntityExists = true;
                        }
                    }
                }
                else if (_race == RaceType::Zerg)
                {
                    for (auto unitIterator : _gameState->unitList)
                    {
                        if ((unitIterator->getName().compare("Queen") == 0))
                        {
							// Queen starts out with 50 energy
							_energy += 50.f;

                            _energyLimit = 200.f;
                            if (_energy < _energyLimit)
                            {
                                _energy += 0.5625f;
                            }
                            _energyEntityExists = true;
                        }
                    }
                }
            }
            else
            {
                if (_energy < _energyLimit)
                {
                    _energy += 0.5625f;
                    _gameState->setEnergy(_energy);
                }
            }

        }

        void handleAbility()
        {
            if (_energyEntityExists)
            {
                if (_race == RaceType::Terran)
                {
                    if (_energy >= 50.f && _muleTimer == 0)
                    {
                         PROGRESS("GSU Mule timer started");
                        _muleTimer = 90;
                        _energy -= 50.f;
                    }

                    if (_muleTimer > 0)
                    {
						// Project1-Description: "roughly 4 times the yield of a SCV"
                        PROGRESS("GSU Mule harvests");
                        _gameState->addMinerals(2.8f);
                        _muleTimer--;
                    }
                }
                else if (_race == RaceType::Zerg)
                {
                    if (_energy >= 25.f && _queenTimer == 0)
                    {
						int larvaCount = 0;
						for (auto unitIterator : _gameState->unitList)
						{
							if ((unitIterator->getName().compare("Larva") == 0))
							{
								larvaCount++;
							}
						}

						if (larvaCount < 19)
						{
							// start the larva injection process
							// decrease energy
							_energy -= 25.f;
							_queenTimer = 40;
							PROGRESS("GSU Queen timer started");
						}
                    }

                    if(_queenTimer > 0)
                    {
                        // decrease queenTimer
                        _queenTimer--;
                        // spawn larva
                        if(_queenTimer == 0)
                        {
                            PROGRESS("GSU Larvas injected by Queen");

							// now we have to ensure the 19 larvae limit
							// first count larvae
							int larvaCount = 0;
							for (auto unitIterator : _gameState->unitList)
							{
								if ((unitIterator->getName().compare("Larva") == 0))
								{
									larvaCount++;
								}
							}

							int injectionCount = 0;
							while (larvaCount < 19 && injectionCount < 4)
							{
								_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
								_technologyManager->notifyCreation("Larva");
								injectionCount++;
							}
                         }
                    }
                }
                else if (_race == RaceType::Protoss)
                {
                     std::vector< std::shared_ptr<Building> > eraseBoostedList;
                    for(auto boostedBuildingIterator : boostedBuildingList)
                    {
                        if(boostedBuildingIterator->boostTimer == 0)
                        {
                            PROGRESS("GSU boostTimer == 0");
                            boostedBuildingIterator->boostState = Building::BoostState::NotBoosted;
                            eraseBoostedList.push_back(boostedBuildingIterator);
                        }
                    }
                    for(auto eraseBoostedIterator : eraseBoostedList)
                    {
                        auto eraseIterator = std::find(boostedBuildingList.begin(), boostedBuildingList.end(), eraseBoostedIterator);
                        if(eraseIterator != boostedBuildingList.end())
                        {
                            PROGRESS("GSU Building not Boosted anymore " << (*eraseIterator)->getName());
                            boostedBuildingList.erase(eraseIterator);
                        }
                    }

                }
            }
        }

        void handleBoost(std::shared_ptr<Building> building)
        {
            if (_race == RaceType::Protoss)
            {
                if (_energy >= 25.f )
                {
                    if(building->boostState == Building::BoostState::NotBoosted)
                    {
                        PROGRESS("GSU Building Boosted " << (building)->getName());
                        building->boostState = Building::BoostState::Boosted;
                        building->boostTimer = 20;
                        boostedBuildingList.push_back(building);
                        _energy-=25.f;
                    }
                }
            }
        }

};

#endif // ENERGIZER_H

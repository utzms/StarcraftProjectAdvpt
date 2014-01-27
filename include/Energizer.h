#ifndef ENERGIZER_H
#define ENERGIZER_H

#include "GameState.h"

template
<
        class RacePolicy
>
class Energizer
{
    private:
        std::shared_ptr<GameState>  _gameState;
        float                       _energy;
        int                         _timer;
        RaceType                    _race;

        bool    _energyEntityExists;
        float   _energyLimit;

        int  _muleTimer;
        int  _queenTimer;

        int _abilityCooldown;
        std::vector< std::shared_ptr<Building> > boostedBuildingList;
    public:
        Energizer(std::shared_ptr<GameState> gameState)
            :_gameState(gameState)
            ,_energy(0)
            ,_timer(0)
            ,_race(RacePolicy::getRace())
            ,_energyEntityExists(false)
            ,_energyLimit(0)
            ,_muleTimer(0)
            ,_abilityCooldown(0)
            ,_queenTimer(0)
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
                        _muleTimer = 90;
                        _energy -= 50.f;
                    }

                    if (_muleTimer > 0)
                    {
                        _gameState->addMinerals(2.67f);
                        _muleTimer--;
                    }
                }
                else if (_race == RaceType::Zerg)
                {
                    if (_energy >= 25.f && _queenTimer == 0)
                    {
                        // decrease energy
                        _energy -= 25.f;
                        _queenTimer = 40;
                    }

                    if(_queenTimer > 0)
                    {
                        // decrease queenTimer
                        _queenTimer--;
                        // spawn larva
                        if( _queenTimer == 0 )
                        {
                            _gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
                            _gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
                            _gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
                            _gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
                         }
                    }
                }
                else if (_race == RaceType::Protoss)
                {
                     std::vector< std::shared_ptr<Building> > eraseBoostedList;
                    for(auto boostedBuildingIterator : boostedBuildingList)
                    {
                        if(boostedBuildingIterator->timer == 0)
                        {
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

        void handleBoost(Building& building)
        {
            if (_race == RaceType::Protoss)
            {
                 PROGRESS("GSU Building Boosted " << (building)->getName());
                building.boostState = Building::BoostState::Boosted;
                building.boostTimer = 20;
                boostedBuildingList.push_back(std::shared_ptr(building));
            }
        }

};

#endif // ENERGIZER_H

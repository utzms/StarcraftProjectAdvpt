#include "../include/Simulation.h"

template <class TechTree>
Simulation<TechTree>::Simulation(std::string buildListFilename,
                                       std::shared_ptr<GameState> gameState,
                                       std::shared_ptr<ResourceManager> resourceManager,
                                       std::shared_ptr<TechnologyManager<TechTree> > technologyManager)
    :_buildList(new BuildList(buildListFilename))
    ,_gameState(gameState)
    ,_resourceManager(resourceManager)
    ,_technologyManager(technologyManager)
{

}

template <class TechTree>
void Simulation<TechTree>::run()
{



}

template <class TechTree>
void Simulation<TechTree>::startSimulation()
{
	do	
	{
        std::cout <<  _buildList->current() << std::endl;
	}
	while( _buildList->advance() != BuildList::State::Finished );

}

template <class TechTree>
void Simulation<TechTree>::timeStep()
{
    std::string currentEntity = _buildList->current();
}

#include <iomanip> //include this for formatted output

#include "../include/Simulation.h"
#include "../include/TechnologyList.h"
#include "../include/InitTechTree.hpp"
#include "../include/Technology.h"
#include "../include/TechnologyManager.h"
#include "../include/techTreePolicy.h"


int main()
{
	//Simulation simulation;
    std::shared_ptr<GameState> gameState(new GameState());
    std::shared_ptr<TechnologyList> technologyList(new TechnologyList());
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 1.f, 1.f));
	std::shared_ptr<TechnologyManager<ProtossTechTree> > techManager(new TechnologyManager<ProtossTechTree>(gameState, technologyList));
	std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("./data/StartingConfiguration.txt")) );

	Simulation<ProtossTechTree> simulation("protoss1.txt", gameState, resourceManager, techManager, startingConfiguration);

    simulation.run();

	return 0;
}

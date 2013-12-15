
#ifndef TECHTREETEST

#include <iomanip> //include this for formatted output

#include "../include/Simulation.h"
#include "../include/TechnologyList.h"
#include "../include/ResourceManager.h"
#include "../include/InitTechTree.hpp"
#include "../include/Technology.h"
#include "../include/TechnologyManager.h"
#include "../include/GameState.h"
#include "../include/GameStateUpdate.h"
int main()
{
    std::string unitPath = "./data/Protoss/units.txt";
    std::string buildingPath = "./data/Protoss/buildings.txt";

	std::shared_ptr<GameState> gameState(new GameState());
	std::shared_ptr<TechnologyList> technologyList(new TechnologyList());
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 1.f, 1.f));
	std::shared_ptr<TechnologyManager> techManager(new TechnologyManager(gameState, unitPath, buildingPath));
    std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("./data/StartingConfiguration.txt")) );
	std::shared_ptr<GameStateUpdate> gameStateUpdate(new GameStateUpdate(gameState,techManager));

   	Simulation simulation("buildlists/protoss1.txt", gameState, resourceManager, techManager, startingConfiguration, gameStateUpdate);

	return 0;
}

#else

#include "Simulation.cpp"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::cerr << "Too few arguments" << std::endl;
	}
	std::string argument(argv[1]);
	Simulation<Terran> simulation(argument);
	simulation.run();

	return 0;
}

#endif

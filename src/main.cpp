#include <iomanip> //include this for formatted output

#include "../include/Simulation.h"
#include "../include/TechnologyList.h"
#include "../include/InitTechTree.hpp"
#include "../include/Technology.h"
#include "../include/TechnologyManager.h"
#include "../include/GameState.h"
#include "../include/GameStateUpdate.h"
int main()
{
    std::string unitPath = "../data/Protoss/units.txt";
    std::string buildingPath = "../data/Protoss/buildings.txt";

	std::shared_ptr<GameState> gameState(new GameState());
	std::shared_ptr<TechnologyList> technologyList(new TechnologyList(buildingPath,unitPath));
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 1.f, 1.f));
	std::shared_ptr<TechnologyManager> techManager(new TechnologyManager(gameState, technologyList));
    std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("../data/StartingConfiguration.txt")) );
	std::shared_ptr<GameStateUpdate> gameStateUpdate(new GameStateUpdate(gameState));

    Simulation simulation("protoss1.txt", gameState, resourceManager, techManager, startingConfiguration, gameStateUpdate);

	simulation.run();

	return 0;
}

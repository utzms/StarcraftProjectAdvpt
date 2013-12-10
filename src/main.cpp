#include <iomanip> //include this for formatted output

#include "../include/Simulation.h"
#include "../include/TechnologyList.h"
#include "../include/InitTechTree.hpp"
#include "../include/Technology.h"
#include "../include/TechnologyManager.h"

int main()
{
	//Simulation simulation;
    std::shared_ptr<GameState> gameState(new GameState());
    std::shared_ptr<TechnologyList> technologyList(new TechnologyList());
    std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 0.6f, 0.6f));
    std::shared_ptr<TechnologyManager> techManager(new TechnologyManager(gameState, technologyList));

    Simulation simulation("../../trunk/protoss1.txt", gameState, resourceManager);

    simulation.run();

	return 0;
}

#include "../src/BuildListOptimizer.cpp"
#include "../src/Simulation.cpp"
#include "../src/BuildListGenerator.cpp"

template class Simulation<Protoss>;
template class Simulation<Zerg>;
template class Simulation<Terran>;

template class BuildListGenerator<Protoss>;
template class BuildListGenerator<Terran>;
template class BuildListGenerator<Zerg>;

template class BuildListOptimizer<Terran, Rush>;
template class BuildListOptimizer<Protoss, Rush>;
template class BuildListOptimizer<Zerg, Rush>;
template class BuildListOptimizer<Terran, Push>;
template class BuildListOptimizer<Protoss, Push>;
template class BuildListOptimizer<Zerg, Push>;

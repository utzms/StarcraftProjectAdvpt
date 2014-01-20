#include "BuildListOptimizer.h"


using std::pair;
using std::map;

inline void dummy()
{
        BuildListOptimizer<Terran, Rush> dummy1();
        BuildListOptimizer<Protoss, Rush> dummy2();
        BuildListOptimizer<Zerg, Rush> dummy3();
        BuildListOptimizer<Terran, Push> dummy4();
        BuildListOptimizer<Protoss, Push> dummy5();
        BuildListOptimizer<Zerg, Push> dummy6();
        BuildListOptimizer<Terran, Debug> dummy7();
        BuildListOptimizer<Protoss, Debug> dummy8();
        BuildListOptimizer<Zerg, Debug> dummy9();
}


template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(string target, int ntargets, int timeLimit, int reproductionRate)
{
    if(reproductionRate > mAccuracy)
    {
        throw std::invalid_argument("The reproduction Rate must be lower or equal the maximum value. Choose a lower rate or adapt the accuracy.");
    }
    std::minstd_rand0 generator1(std::chrono::system_clock::now().time_since_epoch().count());
    std::minstd_rand0 generator2(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> distribution1(0,mPopulation.size()-1);
    std::uniform_int_distribution<int> distribution2(0,1);
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    auto chooseIndividual = std::bind(distribution1, generator1);
    auto coin =  std::bind(distribution2, generator2);


    int ncrossovers = mPopulation.size()*reproductionRate / mAccuracy;
    set<size_t> positions;
    while(--ncrossovers >= 0)
    {
        do
        {
            positions.insert(chooseIndividual());
        } while(positions.size() < 4);

        size_t maxDist = 0, currDist = 0, pos1 = 0, pos2 = 0;
        for(size_t i : positions)
        {
            for(size_t j : positions)
            {
                if(i != j)
                {
                    currDist = calculateDistance(mPopulation[i], mPopulation[j]);
                    if(currDist > maxDist)
                    {
                        maxDist = currDist;
                        pos1 = i;
                        pos2 = j;
                    }
                }
            }
        }
        const vector<string>& mumsGenes = mPopulation[pos1].genes;
        const vector<string>& dadsGenes = mPopulation[pos2].genes;

        vector<string> newGenes(std::max(mumsGenes.size(),dadsGenes.size()));
        size_t i = 0;
        for(;i < std::min(mumsGenes.size(),dadsGenes.size()); ++i)
        {
            int coinFlip = coin();
            if(coinFlip == 1)
            {
                newGenes[i] = dadsGenes[i];
            }
            else
            {
                newGenes[i] = mumsGenes[i];
            }
        }
        const vector<string>& rest = mumsGenes.size() < dadsGenes.size() ? dadsGenes : mumsGenes;
        for(;i < rest.size(); ++i)
        {
            newGenes[i] = rest[i];
        }

        shared_ptr<BuildList> buildList = std::make_shared<BuildList>(newGenes);
        map<int,string> simRes = Simulation<RacePolicy>(buildList, mTechManager.getTechnologyList()).run(timeLimit);
        mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                         fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));

        positions.clear();
    }


}

template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(string target, int ntargets, int timeLimit, int mutationRate)
{
    if(mutationRate > mAccuracy)
    {
        throw std::invalid_argument("The mutation Rate must be lower or equal the maximum value. Choose a lower rate or adapt the accuracy.");
    }

    std::minstd_rand0 popGen(std::chrono::system_clock::now().time_since_epoch().count());
    std::minstd_rand0 geneGen(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<size_t> popDist(0,mPopulation.size()-1);
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);

    auto chooseIndividual = std::bind(popDist, popGen);

    int nmutants = mPopulation.size() / 5;
    while(--nmutants >= 0)
    {

        const Individual& oldInd = mPopulation[chooseIndividual()];
        vector<string> newGenes;
        do
        {
            newGenes = oldInd.genes;
            std::uniform_int_distribution<size_t> geneDist(0,newGenes.size()-1);
            auto chooseGene = std::bind(geneDist, geneGen);
            for(int i = 0; i < (mutationRate * newGenes.size())/mAccuracy; ++i)
            {

                geneDist = std::uniform_int_distribution<size_t>(0,newGenes.size()-1);
                newGenes.erase(newGenes.begin()+chooseGene());

                geneDist = std::uniform_int_distribution<size_t>(0,newGenes.size()-1);
                string entry = mBuildListGen.getRandomTechnologyName();
                newGenes.insert(newGenes.begin()+chooseGene(), entry);
            }
        } while(mTechManager.isBuildListPossible(newGenes));
        shared_ptr<BuildList> buildList = std::make_shared<BuildList>(newGenes);
        map<int,string> simRes = Simulation<RacePolicy>(buildList, mTechManager.getTechnologyList()).run(timeLimit);
        mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                         fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
    }
}

template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(int selectionRate)
{
    if(selectionRate > mAccuracy)
    {
        throw std::invalid_argument("The selection Rate must be lower or equal the maximum value. Choose a lower rate or adapt the accuracy.");
    }
    int threshold = (mPopulation.size()*selectionRate)/mAccuracy;
    if (threshold <= 0)
        return;
    mPopulation.erase(mPopulation.begin(), mPopulation.end()-threshold);
}

template <class RacePolicy, class FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(int accuracy, size_t individualSize)
    : mAccuracy(accuracy), mIndividualSize(individualSize)
{
    mTechManager = TechnologyManager<RacePolicy>();
    mBuildListGen = BuildListGenerator<RacePolicy>(mTechManager.getTechnologyList());
    mBuildListGen.initRandomGenerator();
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(string target, int ntargets, int timeLimit, int initPopSize)
{
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    for(int i = mPopulation.size(); i < initPopSize; ++i)
    {
        shared_ptr<BuildList> bl = mBuildListGen.buildOneRandomList(mIndividualSize);
        map<int,string> simRes = Simulation<RacePolicy>(bl, mTechManager.getTechnologyList()).run(timeLimit);
        mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                         fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), bl->getAsVector()));
    }
    std::sort(mPopulation.begin(), mPopulation.end());
}


/* clears the whole population by terminating (sic!) all individuals */
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clear(void)
{
    mPopulation.clear();
}

/* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(string target, int ntargets, int timeLimit, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
    while(--generations >= 0)
    {
        select(selectionRate);
        mutate(target, ntargets, timeLimit, mutationRate);
        crossover(target, ntargets, timeLimit, reproductionRate);
        sortPopulation();
    }
    std::cout << mPopulation.size() << std::endl;
}


/* combined use of initialize and optimize */
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clearInitializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
    clear();
    initialize(target, ntargets, timeLimit, initPopSize);
    optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);

}

/* combined use of initialize and optimize */
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
    initialize(target, ntargets, timeLimit, initPopSize);
    optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);
}

/* adds a specific individual to the population */
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::addIndividual(string target, int ntargets, int timeLimit, shared_ptr<BuildList> buildList )
{
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    map<int,string> simRes = Simulation<RacePolicy>(buildList, mTechManager.getTechnologyList()).run(timeLimit);
    mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                     fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
}

/* get the group of size number of the fittest individuals, together with their corresponding fitness value */
template <class RacePolicy, class FitnessPolicy>
vector<Individual> BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestGroup(int groupSize)
{
    sortPopulation();
    vector<Individual> res;
    if (mPopulation.size() < groupSize)
    {
        throw std::invalid_argument("@BuildListOptimizer::getFittestGroup: Lower population than requested groupSize!");
    }
    for (int i = mPopulation.size()-1; i >= mPopulation.size()-groupSize; --i)
    {
        res.push_back(mPopulation.at(i));
    }
    return res;
}

/* get the overall fittest individual */
template <class RacePolicy, class FitnessPolicy>
Individual BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestIndividual(void)
{
    sortPopulation();
    return *(mPopulation.rbegin());
}

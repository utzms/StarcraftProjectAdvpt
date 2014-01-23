#include "BuildListOptimizer.h"


using std::pair;
using std::map;
using std::future;
using std::async;

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
        throw std::invalid_argument("The reproduction Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(reproductionRate));
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
        PROGRESS("Mother:" << "\n\n" << mPopulation[pos1] << "\n" << std::endl);
        PROGRESS("Father:" << "\n\n" << mPopulation[pos2] << "\n" << std::endl);

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

        Individual child(fitnessPolicy.rateBuildListHard(simRes), fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector());
        mPopulation.push_back(child);

        PROGRESS("Child:" << "\n\n" << child << std::endl << "-----------------------------------------------------------" << std::endl);

        positions.clear();
    }


}

template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(string target, int ntargets, int timeLimit, int mutationRate)
{
    if(mutationRate > mAccuracy)
    {
        throw std::invalid_argument("The mutation Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(mutationRate));
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
        PROGRESS("Original Individual:" << "\n\n" << oldInd << "\n" << std::endl);
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
        Individual mutant(fitnessPolicy.rateBuildListHard(simRes), fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector());
        mPopulation.push_back(mutant);

        PROGRESS("Mutant:" << "\n\n" << mutant << std::endl << "-----------------------------------------------------------" << std::endl);

    }
}

template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(int selectionRate)
{
    if(selectionRate > mAccuracy)
    {
        throw std::invalid_argument("@BuildListOptimizer::select: The selection Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(selectionRate));
    }
    int threshold = (mPopulation.size()*selectionRate)/mAccuracy;
    if (threshold <= 0)
        return;
    mPopulation.erase(mPopulation.begin(), mPopulation.end()-threshold);
}

template <class RacePolicy, class FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(int accuracy, int individualSize)
{
    if(accuracy <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::BuildListOptimizer(int accuracy, size_t individualSize): The accuracy must be greater than zero. The value passed is: "+std::to_string(accuracy));
    }
    if(individualSize <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::BuildListOptimizer(int accuracy, size_t individualSize): The size of the individuals must be greater than zero. The value passed is: "+std::to_string(accuracy));
    }

    mAccuracy = accuracy;
    mIndividualSize = individualSize;

    mTechManager = TechnologyManager<RacePolicy>();
    mBuildListGen = BuildListGenerator<RacePolicy>(mTechManager.getTechnologyList());
    mBuildListGen.initRandomGenerator();
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(string target, int ntargets, int timeLimit, int initPopSize)
{

    if(!mTechManager.technologyExists(target))
    {
        throw std::invalid_argument("@BuildListOptimizer::initialize: "+target+" is not existent in the tech tree.");
    }
    else if(ntargets <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::initialize: The number of targets that shall be produced must be greater than zero. The passed value is: "+std::to_string(ntargets));
    }
    else if(timeLimit <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::initialize: The time limit mst be greater than zero. The passed value is: "+std::to_string(timeLimit));
    }
    else if(initPopSize <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::initialize: The initial population size must be greater zero. The passed value is: "+std::to_string(initPopSize));
    }
    #ifdef DEBUG
        int failedThreadCount = 0;
    #endif


    vector<future<shared_ptr<BuildList>>> buildListFutureVec(initPopSize-mPopulation.size());
    vector<future<map<int,string>>> resultFutureVec(initPopSize-mPopulation.size());
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    auto genBuildList = [=] () -> shared_ptr<BuildList> { return mBuildListGen.buildOneRandomList(mIndividualSize); };
    auto runSimulation = [=] (shared_ptr<BuildList> bl, TechnologyList techList) -> map<int,string> { return Simulation<RacePolicy>(bl, techList).run(timeLimit); };

    for(int i = 0; i < buildListFutureVec.size(); ++i)
    {

        buildListFutureVec[i] = async(genBuildList);
    }

    vector<shared_ptr<BuildList>> bls(buildListFutureVec.size());
    for(int i = 0; i < bls.size(); ++i)
    {

        try
        {
            bls[i] = buildListFutureVec[i].get();
        }
        catch(std::system_error& e)
        {
            PROGRESS("@BuildListOptimizer::initialize: system_error caught, unable to start new thread to generate BuildList");
            #ifdef DEBUG
                ++failedThreadCount;
            #endif
            bls[i] = genBuildList();
        }

        resultFutureVec[i] = async(runSimulation, bls[i], mTechManager.getTechnologyList());
    }

    for(int i = 0; i < resultFutureVec.size(); ++i)
    {
        map<int,string> simRes;
        try
        {
            simRes = resultFutureVec[i].get();
        }
        catch(std::system_error& e)
        {
            PROGRESS("@BuildListOptimizer::initialize: system_error caught, unable to start new thread to run Simulation");
            #ifdef DEBUG
                ++failedThreadCount;
            #endif
            simRes = runSimulation(bls[i], mTechManager.getTechnologyList());
        }

        mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                         fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), bls[i]->getAsVector()));
    }


    std::sort(mPopulation.begin(), mPopulation.end());
    #ifdef DEBUG
        std::cerr << "@BuildListOptimizer::initialize: Failed to start thread for " << std::to_string(failedThreadCount) << " times." << std::endl;
    #endif

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
    if(!mTechManager.technologyExists(target))
    {
        throw std::invalid_argument("@BuildListOptimizer::optimize: "+target+" is not existent in the tech tree.");
    }
    else if(ntargets <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::optimize: The number of targets that shall be produced must be greater than zero. The passed value is: "+std::to_string(ntargets));
    }
    else if(timeLimit <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::optimize: The time limit must be greater than zero. The passed value is: "+std::to_string(timeLimit));
    }
    else if(generations <= 0)
    {
        throw std::invalid_argument("@BuildListOptimizer::optimize: The number of generations must be greater than zero. The passed value is: "+std::to_string(generations));
    }

    while(--generations >= 0)
    {
        select(selectionRate);
        mutate(target, ntargets, timeLimit, mutationRate);
        crossover(target, ntargets, timeLimit, reproductionRate);
        sortPopulation();
    }

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

    if (mPopulation.size() < groupSize)
    {
        throw std::invalid_argument("@BuildListOptimizer::getFittestGroup: Lower population than requested groupSize. The value passed is: "+std::to_string(groupSize));
    }

    sortPopulation();
    vector<Individual> res;
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

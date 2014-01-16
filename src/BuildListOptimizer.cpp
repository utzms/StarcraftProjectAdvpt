#include "../include/BuildListOptimizer.h"


using std::pair;

inline void dummy()
{
        BuildListOptimizer<TerranPolicy, RushPolicy> dummy1();
        BuildListOptimizer<ProtossPolicy, RushPolicy> dummy2();
        BuildListOptimizer<ZergPolicy, RushPolicy> dummy3();
        BuildListOptimizer<TerranPolicy, PushPolicy> dummy4();
        BuildListOptimizer<ProtossPolicy, PushPolicy> dummy5();
        BuildListOptimizer<ZergPolicy, PushPolicy> dummy6();
}


template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(string target, int ntargets, int timeLimit, int reproductionRate)
{
        std::minstd_rand0 randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
        if(reproductionRate >= accuracy)
        {
                throw std::invalid_argument("The reproduction Rate must be lower than the maximum value. Choose a lower rate or adapt the accuracy.");
        }
        int ncrossovers = mPopulation.size()*reproductionRate / mAccuracy;
        set<size_t> pos;
        while(--ncrossovers <= 0)
        {
                do
                {
                        pos.insert(randomEngine() % mPopulation.size());
                } while(pos.size() < 4);

         // TODO implement crossover

                pos.clear();
        }

}

template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(string target, int ntargets, int timeLimit, int mutationRate)
{

        std::minstd_rand0 randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
        if(mutationRate >= accuracy)
        {
                throw std::invalid_argument("The mutation Rate must be lower than the maximum value. Choose a lower rate or adapt the accuracy.");
        }
        int nmutants = mPopulation.size() / 5;
        while(--nmutants <= 0)
        {
                const Individual& oldInd = mPopulation[randomEngine() % mPopulation.size()];
                vector<string> newGenes = oldInd.genes;
                for(int i = 0; i < (mutationRate * newGenes.size())/mAccuracy; ++i)
                {
                        newGenes.erase(randomEngine() % newGenes.size());
                        newGenes.insert(randomEngine() % newGenes.size());
                }
                shared_ptr<BuildList> buildList = std::make_shared(new BuildList(newGenes));
                map<int,string> simRes = Simulation(buildList, mTechManager.getTechnologyList()).run(timeLimit);
                mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                                 fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
        }
}

template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(int selectionRate)
{
        if(selectionRate >= accuracy)
        {
                throw std::invalid_argument("The selection Rate must be lower than the maximum value. Choose a lower rate or adapt the accuracy.");
        }
        int threshold = (mPopulation.size()*selectionRate)/mAccuracy;
        mPopulation.erase(mPopulation.begin(), mPopulation.end()-threshold);
}

template <RacePolicy, FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(int accuracy, size_t individualSize)
    : mAccuracy(accuracy), mIndividualSize(individualSize)
{
    mTechManager();
    mBuildListGen(mTechManager.getTechnologyList());
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(string target, int ntargets, int timeLimit, int initPopSize)
{
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    for(int i = mPopulation.size(); i < initPopSize; ++i)
    {
        shared_ptr<BuildList> bl = mBuildListGen.getOneRandomList(mIndividualSize);
        map<int,string> simRes = Simulation(bl, mTechManager.getTechnologyList()).run(timeLimit);
        mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                         fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), bl->getAsVector()));
    }
    std::sort(mPopulation.begin(), mPopulation.end());
}


/* clears the whole population by terminating (sic!) all individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clear(void)
{
        mPopulation.clear();
}

/* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(string target, int ntargets, int timeLimit, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
        //TODO

}


/* combined use of initialize and optimize */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clearInitializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
        //TODO

}

/* combined use of initialize and optimize */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{
        //TODO

}

/* adds a specific individual to the population */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::addIndividual(string target, int ntargets, int timeLimit, shared_ptr<BuildList> buildList)
{
    FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    map<int,string> simRes = Simulation(buildList, mTechManager.getTechnologyList()).run(timeLimit);
    mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
                                     fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
}

/* get the group of size number of the fittest individuals, together with their corresponding fitness value */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestGroup(int groupSize, vector< pair<int,BuildList> >& res)
{
        //TODO

}

/* get the overall fittest individual */
template <RacePolicy, FitnessPolicy>
shared_ptr< BuildList > BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestIndividual(void)
{
        //TODO

}

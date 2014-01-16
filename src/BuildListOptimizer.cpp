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
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(string target, int nTargets, int timeLimit, int reproductionRate)
{

}

template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(string target, int nTargets, int timeLimit, int mutationRate)
{
        if(mutationRate >= accuracy)
        {
                throw std::invalid_argument("The mutation Rate must be lower than the maximum value. Choose a lower rate or adapt the accuracy.");
        }
        int nmutants = mPopulation.size()*mutationRate / mAccuracy;

}

template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(int selectionRate)
{
        if(selectionRate >= accuracy)
        {
                throw std::invalid_argument("The selection Rate must be lower than the maximum value. Choose a lower rate or adapt the accuracy.");
        }
        int threshold = (mPopulation.size()*selectionRate)/mAccuracy;
        mPopulation.erase(mPopulation.lower_bound(threshold), mPopulation.end());
}

template <RacePolicy, FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(int accuracy, size_t individualSize)
    : mAccuracy(accuracy), mIndividualSize(individualSize)
{
    mBuildListGen(mTechManager.getTechnologyList());
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(string target, int nTargets, int timeLimit, int initPopSize)
{
    FitnessPolicy fitnessPolicy(target, timeLimit, nTargets);
    for(int i = mPopulation; i < initPopSize; ++i)
    {
        shared_ptr<BuildList> bl = mBuildListGen.getOneRandomList(mIndividualSize);
        map<int,string> simRes = Simulation(bl, mTechList).run(timeLimit);
        mPopulation.insert(std::pair<int,BuildList>(fitnessPolicy.rateBuildListHard(simRes),*bl));
    }
}


/* clears the whole population by terminating (sic!) all individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clear(void)
{
        mPopulation.clear();
}

/* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(string target, int nTargets, int timeLimit, int generations, int reproductionRate, int mutationRate, int selectionRate)
{

}


/* combined use of initialize and optimize */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clearInitializeAndOptimize(string target, int nTargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{

}

/* combined use of initialize and optimize */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(string target, int nTargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate)
{

}

/* adds a specific individual to the population */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::addIndividual(BuildList buildlist)
{

}

/* get the group of size number of the fittest individuals, together with their corresponding fitness value */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestGroup(int groupSize, vector< pair<int,BuildList> >& res)
{

}

/* get the overall fittest individual */
template <RacePolicy, FitnessPolicy>
shared_ptr< BuildList > BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestIndividual(void)
{

}

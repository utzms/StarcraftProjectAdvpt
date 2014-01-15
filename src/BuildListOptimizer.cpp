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
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(float reproductionRate)
{

}
template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(float mutationRate)
{

}
template <RacePolicy, FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(float selectionRate)
{

}


template <RacePolicy, FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(string target, int timeLimit, size_t individualSize)
        : nTimeLimit(timeLimit), nIndividualSize(individualSize)
{
        if(!InitTechTree<RacePolicy>(&mTechList).initTechTree())
        {
            throw std::runtime_error("TechnologyList initialization failed. Something went terribly wrong!");
        }
        mBuildListGen(mTechList);

}


/*initializes the population with random individuals until the population size reaches initPopSize*/
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(int initPopSize)
{
        FitnessPolicy fitnessPolicy(mTarget, mTimeLimit, mNumber);
        for(int i = mPopulation; i < initPopSize; ++i)
        {
                shared_ptr<BuildList> bl = mBuildListGen.getOneRandomList(mIndividualSize);
                map<int,string> simRes = Simulation(bl, mTechList).runAndGetResult();
                mPopulation.insert(std::pair<fitnessPolicy.rateBuildListHard(simRes));
        }


}

/* clears the whole population by terminating (sic!) all individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clear(void)
{

}

/* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(int generations, float reproductionRate, float mutationRate, float selectionRate)
{

}

/* combined use of initialize and optimize */
template <RacePolicy, FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(int initPopSize, int generations, float reproductionRate, float mutationRate, float selectionRate)
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

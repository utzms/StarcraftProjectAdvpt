#ifndef BUILDLISTOPTIMIZER_H
#define BUILDLISTOPTIMIZER_H

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <chrono>
#include "Simulation.h"
#include "BuildList.h"
#include "BuildListGenerator.h"
#include "TechnologyList.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::multimap;

template <class RacePolicy, class FitnessPolicy>
class BuildListOptimizer : public RacePolicy, public FitnessPolicy
{

private:
        /* naming convention:
          m : member variable
          c : constant
          ...
        */
        multimap< int, vector<string> > mPopulation;
        std::hash<string> mHashGen;
        string mTarget;
        int mNumber;
        int mTimeLimit;
        size_t mIndividualSize;
        TechnologyList<RacePolicy> mTechList;
        BuildListGenerator<RacePolicy> mBuildListGen;


        inline void crossover(float reproductionRate);
        inline void mutate(float mutationRate);
        inline void select(float selectionRate);


public:

        BuildListOptimizer(string target, int number, int timeLimit, size_t individualSize);
        BuildListOptimizer(string target, int number, size_t individualSize);
        BuildListOptimizer(string target, int timeLimit, size_t individualSize);
        BuildListOptimizer() {} // dummy Default-Constructor

        void setIndividualSize(size_t newSize)
        {
            mIndividualSize = newSize;
        }

        void setTarget(string target)
        {
            mTarget = target;
        }

        void setNumber(int number)
        {
            mNumber = number;
        }

        void setTimeLimit(int timeLimit)
        {
            mTimeLimit = timeLimit;
        }

        size_t getIndividualSize()
        {
            return mIndividualSize;
        }

        string getTarget()
        {
            return mTarget;
        }

        int getNumber()
        {
            return mNumber;
        }

        int getTimeLimit()
        {
            return mTimeLimit;
        }


        /*initializes the population with random individuals until the population size reaches initPopSize*/
        void initialize(int initPopSize);

        /* clears the whole population by terminating (sic!) all individuals */
        void clear(void);

        /* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
        void optimize(int generations, float reproductionRate, float mutationRate, float selectionRate);

        /* combined use of initialize and optimize */
        void initializeAndOptimize(int initPopSize, int generations, float reproductionRate, float mutationRate, float selectionRate);

        /* adds a specific individual to the population */
        void addIndividual(BuildList buildlist);

        /* get the group of size number of the fittest individuals, together with their corresponding fitness value */
        void getFittestGroup(int groupSize, vector< pair<int,BuildList> >& res);

        /* get the overall fittest individual */
        shared_ptr< BuildList > getFittestIndividual(void);

};


#endif // BUILDLISTOPTIMIZER_H

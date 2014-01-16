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

struct Individual
{
    int hardSkills;
    int softSkills;
    vector<string> genes;
    Individual(int fitness_init, vector<string> genes_init)
        : fitness(fitness_init), genes(genes_init)
    {}
    bool operator>(Individual ind1, Individual ind2)
    {
        if(ind1.hardSkills > ind2.hardSkills)
        {
            return true;
        }
        return ind1.softSkills > ind2.softSkills;
    }

    bool operator<(Individual ind1, Individual ind2)
    {
        if(ind1.hardSkills < ind2.hardSkills)
        {
            return true;
        }
        return ind1.softSkills < ind2.softSkills;

    }
    bool operator==(Individual ind1, Individual ind2)
    {
        return ind1.hardSkills == ind2.hardSkills && ind1.softSkills == ind2.hardSkills;
    }
};

template <class RacePolicy, class FitnessPolicy>
class BuildListOptimizer : public RacePolicy, public FitnessPolicy
{

private:
        /* naming convention:
          m : member variable
          c : constant
          ...
        */
        vector<Individual> mPopulation;
        std::hash<string> mHashGen;
        int mAccuracy;
        size_t mIndividualSize;
        TechnologyManager<RacePolicy> mTechManager;
        BuildListGenerator<RacePolicy> mBuildListGen;


        inline void crossover(string target, int nTargets, int timeLimit, int reproductionRate);
        inline void mutate(string target, int nTargets, int timeLimit, int mutationRate);
        inline void select(int selectionRate);


public:

        BuildListOptimizer(int accuracy, size_t individualSize);
        BuildListOptimizer() {} // dummy Default-Constructor


        void setAccuracy(int accuracy)
        {
                mAccuracy = accuracy;
        }

        int getAccuracy(int accuracy)
        {
                return mAccuracy;
        }

        void setIndividualSize(size_t newSize)
        {
            mIndividualSize = newSize;
        }

        size_t getIndividualSize()
        {
            return mIndividualSize;
        }



        /*initializes the population with random individuals until the population size reaches initPopSize*/
        void initialize(string target, int nTargets, int timeLimit, int initPopSize);

        /* clears the whole population by terminating (sic!) all individuals */
        void clear(void);

        /* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
        void optimize(string target, int nTargets, int timeLimit, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* combined use of initialize and optimize */
        void initializeAndOptimize(string target, int nTargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* combined use of clear, initialize and optimize */
        void clearInitializeAndOptimize(string target, int nTargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* adds a specific individual to the population */
        void addIndividual(vector<std::string> individiual);

        /* get the group of size number of the fittest individuals, together with their corresponding fitness value */
        void getFittestGroup(int groupSize, vector< pair<int,BuildList> >& res);

        /* get the overall fittest individual */
        shared_ptr< BuildList > getFittestIndividual(void);

};


#endif // BUILDLISTOPTIMIZER_H

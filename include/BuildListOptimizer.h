#ifndef BUILDLISTOPTIMIZER_H
#define BUILDLISTOPTIMIZER_H

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <chrono>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <set>
#include "Simulation.h"
#include "BuildList.h"
#include "BuildListGenerator.h"
#include "TechnologyList.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::multimap;
using std::set;

struct Individual
{
    int hardSkills;
    int softSkills;
    vector<string> genes;

    Individual(int hardSkills_init, int softSkills_init, vector<string> genes_init)
        : hardSkills(fitness_init), softSkills(softSkills_init), genes(genes_init)
    {}



    bool operator>(const Individual& ind1, const Individual& ind2)
    {
        if(ind1.hardSkills > ind2.hardSkills)
        {
            return true;
        }
        return ind1.hardSkills == ind2.hardSkills ? ind1.softSkills > ind2.softSkills : false;

    }

    bool operator<(const Individual& ind1, const Individual& ind2)
    {
        if(ind1.hardSkills < ind2.hardSkills)
        {
            return true;
        }
        return ind1.hardSkills == ind2.hardSkills ? ind1.softSkills < ind2.softSkills : false;

    }
    bool operator==(const Individual& ind1, const Individual& ind2)
    {
        return ind1.hardSkills == ind2.hardSkills && ind1.softSkills == ind2.hardSkills;
    }

    ostream& operator<<(ostream& out, const Individual& ind)
    {
            for(string gene : ind.genes)
            {
                    out << gene << std::endl;
            }
            return out;
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
        int mAccuracy;
        size_t mIndividualSize;
        TechnologyManager<RacePolicy> mTechManager;
        BuildListGenerator<RacePolicy> mBuildListGen;

        inline void sortPopulation()
        {
                std::sort(mPopulation.begin(), mPopulation.end());
        }
        inline void crossover(string target, int ntargets, int timeLimit, int reproductionRate);
        inline void mutate(string target, int ntargets, int timeLimit, int mutationRate);
        inline void select(int selectionRate);

        inline size_t calculateDistance(const Individual& ind1, const Individual& ind2)
        {
            size_t res;
            std::hash<string> hashGen;
            size_t len = ind1.genes.size() < ind2.genes.size() ? ind1.genes.size() : ind2.genes.size();
            for(size_t i = 0; i < len; ++i)
            {
                res += (len-i) * std::abs(ind1.genes[i]-ind2.genes[i]);
            }
            return res;
        }


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
        void initialize(string target, int ntargets, int timeLimit, int initPopSize);

        /* clears the whole population by terminating (sic!) all individuals */
        void clear(void);

        /* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
        void optimize(string target, int ntargets, int timeLimit, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* combined use of initialize and optimize */
        void initializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* combined use of clear, initialize and optimize */
        void clearInitializeAndOptimize(string target, int ntargets, int timeLimit, int initPopSize, int generations, int reproductionRate, int mutationRate, int selectionRate);

        /* adds a specific individual to the population */
        void addIndividual(string target, int ntargets, int timeLimit, shared_ptr<BuildList> buildList);

        /* get the group of size number of the fittest individuals, together with their corresponding fitness value */
        vector<Individual> getFittestGroup(int groupSize);

        /* get the overall fittest individual */
        Individual getFittestIndividual(void);

};


#endif // BUILDLISTOPTIMIZER_H

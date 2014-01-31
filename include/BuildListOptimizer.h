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
#include <future>
#include <thread>
#include <functional>
#include <iostream>

#include "BuildListGenerator.h"
#include "BuildList.h"
#include "TechnologyManager.h"
#include "RacePolicy.h"
#include "FitnessPolicy.h"
#include "Debug.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::set;
using std::map;

struct Individual
{
    int hardSkills;
    int softSkills;
    vector<string> genes;

    Individual(int hardSkills_init, int softSkills_init, vector<string> genes_init)
        : hardSkills(hardSkills_init), softSkills(softSkills_init), genes(genes_init)
    {}


    bool operator>(const Individual& ind) const
    {
        if(ind.hardSkills < this->hardSkills)
        {
            return true;
        }
        return ind.hardSkills == this->hardSkills ? ind.softSkills < this->softSkills : false;

    }

    bool operator<(const Individual& ind) const
    {
        if(ind.hardSkills > this->hardSkills)
        {
            return true;
        }
        return ind.hardSkills == this->hardSkills ? ind.softSkills > this->softSkills : false;

    }
    bool operator==(const Individual& ind) const
    {
        return ind.hardSkills == this->hardSkills && ind.softSkills == this->hardSkills;
    }


};

inline std::ostream& operator<<(std::ostream& out, const Individual& ind)
{
    size_t count = 0;
    for(string gene : ind.genes)
    {
        out << std::to_string(++count) << ":" << gene << std::endl;
    }
    out << "Score: " << ind.hardSkills << "\t-\t" << ind.softSkills << std::endl;
    return out;
}
inline std::ostream& operator<<(std::ostream& out, const vector<Individual>& inds)
{
    for (auto i : inds)
        out << i << std::endl;
    return out;
}

template <class RacePolicy, class FitnessPolicy>
class BuildListOptimizer : public RacePolicy //, public FitnessPolicy
{

private:
    /* naming convention:
          m : member variable
          c : constant
          ...
        */
    vector<Individual> mPopulation;
    size_t mAccuracy;
    size_t mIndividualSize;
    TechnologyManager<RacePolicy> mTechManager;

    inline void sortPopulation()
    {
        std::sort(mPopulation.begin(), mPopulation.end());
    }
    inline void crossover(string target, size_t ntargets, int timeLimit, size_t reproductionRate);
    inline void mutate(string target, size_t ntargets, int timeLimit, size_t mutationRate);
    inline void select(size_t selectionRate);


    inline void generateAndRate(const string target, FitnessPolicy& fitnessPolicy, const size_t nindividuals, std::function<vector<string>(TechnologyManager<RacePolicy>)> genBuildList, const int timeLimit);

public:

    BuildListOptimizer(const size_t accuracy, const size_t individualSize);
    BuildListOptimizer()// dummy Default-Constructor
    {
        mAccuracy=100;
        mIndividualSize=20;
        mTechManager = TechnologyManager<RacePolicy>();
    }

    void setAccuracy(const size_t accuracy)
    {
        mAccuracy = accuracy;
    }

    size_t getAccuracy()
    {
        return mAccuracy;
    }

    void setIndividualSize(const size_t newSize)
    {
        mIndividualSize = newSize;
    }

    size_t getIndividualSize()
    {
        return mIndividualSize;
    }

    size_t getPopulationSize()
    {
        return mPopulation.size();
    }


    /*initializes the population with random individuals until the population size reaches initPopSize*/
    void initialize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize);

    /* clears the whole population by terminating (sic!) all individuals */
    void clear(void);

    /* optimizes a buildList by mutating, crossing over and selecting the fittest individuals */
    void optimize(const string target, const size_t ntargets, const int timeLimit, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate);

    /* combined use of initialize and optimize */
    void initializeAndOptimize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate);

    /* combined use of clear, initialize and optimize */
    void clearInitializeAndOptimize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate);

    /* adds a specific individual to the population */
    void addIndividual(const string target, const size_t ntargets, const int timeLimit, shared_ptr<BuildList> buildList);

    /* get the group of size number of the fittest individuals, together with their corresponding fitness value */
    vector<Individual> getFittestGroup(const size_t groupSize);

    /* get the overall fittest individual */
    Individual getFittestIndividual(void);

	void printBest(int timeLimit, std::string target);

};


#endif // BUILDLISTOPTIMIZER_H

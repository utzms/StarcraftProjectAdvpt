#ifndef BUILDLISTOPTIMIZER_H
#define BUILDLISTOPTIMIZER_H

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include "Simulation.h"
#include "BuildList.h"


using std::string;
using std::vector;
using std::shared_ptr;

template <class FitnessPolicy>
class BuildlistOptimizer
{

private:
        std::map<int, BuildList> mPopulation;
        std::hash<string> mHashGen;
        int mTimeLimit;
        void initialize(int initPopSize);
        void mutate(float mutationRate);
        void select(float selectionRate);


public:
        BuildlistOptimizer(int timeLimit);
        shared_ptr< vector<string> > optimize(int initPopSize, float mutationRate, float selectionRate);

};


#endif // BUILDLISTOPTIMIZER_H

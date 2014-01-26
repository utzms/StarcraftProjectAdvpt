#include "BuildListOptimizer.h"


using std::pair;
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


	template<class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::generateAndRate(const string target, FitnessPolicy& fitnessPolicy, const int nindividuals, std::function<shared_ptr<BuildList>(TechnologyManager<RacePolicy>)> genBuildList, const int timeLimit)
{
	const TechnologyList& techList = mTechManager.getTechnologyList();
	auto runSimulation = [=] (shared_ptr<BuildList> buildList) -> map<int, string>
	{
		Simulation<RacePolicy> a(buildList, techList);
		return a.run(timeLimit);
	};

	auto runIndividual = [&] (map<int,string> res, shared_ptr<BuildList> bl) -> Individual
	{
		auto a = mTechManager.getEntityRequirements(target);
		Individual newOne(fitnessPolicy.rateBuildListHard(res), fitnessPolicy.rateBuildListSoft(res,RacePolicy::getWorker(), a),bl->getAsVector());
		return newOne;
	};

	vector<future<shared_ptr<BuildList>>> buildListFutureVec(NUM_THREADS);
	vector<future<map<int,string>>> resultFutureVec(NUM_THREADS);
	vector<shared_ptr<BuildList>> bls(nindividuals/NUM_THREADS*(NUM_THREADS+1));
	vector<future<Individual>> individualFutureVec(NUM_THREADS);

    PROGRESS("buildListFutureVec.size() << std::endl");

#ifdef DEBUG
	int threadFailures = 0;
#endif

	for (int i = 0; i < nindividuals; i = i+NUM_THREADS)
	{
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			buildListFutureVec[j] = async(std::launch::async,genBuildList, mTechManager);
		}
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			try
			{
                bls[i+j] = buildListFutureVec[j].get();
#ifdef DEBUG
				if ((i+j)%1000==0)
                    std::cerr << i+j <<"\t Random lists finished\n";
#endif
			}
			catch(std::system_error&)
			{
				bls[i+j] = genBuildList(mTechManager);
#ifdef DEBUG
				std::cerr << "Was unable to start thread, std::system_error caught" << std::endl;
				++threadFailures;
#endif
			}
		}
	}
	for (int i = 0; i < nindividuals; i = i+NUM_THREADS)
	{
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			resultFutureVec[j] = async(std::launch::async,runSimulation, bls[i+j]);
		}
		map<int,string> simRes[NUM_THREADS];
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			try
			{
				simRes[j] = resultFutureVec[j].get();
			}
			catch(std::system_error&)
			{
				simRes[j] = runSimulation(bls[i+j]);
#ifdef DEBUG
				std::cerr << "Was unable to start thread, std::system_error caught" << std::endl;
				++threadFailures;
#endif
            }
#ifdef DEBUG
				if ((i+j)%1000==0)
					std::cout << i+j <<"\t Simulations caught\n";
#endif
		//	Individual newOne(fitnessPolicy.rateBuildListHard(simRes), fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), bls[i+j]->getAsVector());
		//	mPopulation.push_back(newOne);
		}
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			individualFutureVec[j] = async(std::launch::async,runIndividual,simRes[j],bls[i+j]);
		}
		for (int j = 0; j < NUM_THREADS; ++j)
		{
			try
			{
				mPopulation.push_back(individualFutureVec[j].get());
			} catch(std::system_error&)
            {

            }
		}
	}

#ifdef DEBUG
	std::cerr << "Failed to start thread for " << std::to_string(threadFailures) << " times." << std::endl;
#endif
}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(const string target, const int ntargets, const int timeLimit, const int reproductionRate)
{
	if(reproductionRate > mAccuracy)
	{
		throw std::invalid_argument("The reproduction Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(reproductionRate));
	}

	std::minstd_rand0 generator1(std::chrono::system_clock::now().time_since_epoch().count());
	std::minstd_rand0 generator2(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<size_t> distribution1(0,mPopulation.size()-1);
	std::uniform_int_distribution<int> distribution2(0,1);

	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> shared_ptr<BuildList>
	{

		auto chooseIndividual = std::bind(distribution1, generator1);
		auto coin =  std::bind(distribution2, generator2);

		std::hash<string> hashGen;
		set<size_t> positions;
		vector<string> newGenes;
		do {
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
						const Individual& ind1 = mPopulation[i];
						const Individual& ind2 = mPopulation[j];

						size_t res;
						size_t len = ind1.genes.size() < ind2.genes.size() ? ind1.genes.size() : ind2.genes.size();
						for(size_t i = 0; i < len; ++i)
						{
							res += (len-i) * std::abs(hashGen(ind1.genes[i])-hashGen(ind2.genes[i]));
						}


						currDist = res;
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

			newGenes.clear();
			newGenes.resize(std::max(mumsGenes.size(),dadsGenes.size()));
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
			positions.clear();

		} while(!(techManager.isBuildListPossible(newGenes)));
		return std::make_shared<BuildList>(newGenes);
	};

	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	const int ncrossovers = mPopulation.size()*reproductionRate / mAccuracy;
	generateAndRate(target, fitnessPolicy, ncrossovers, genBuildList, timeLimit);

}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(const string target, const int ntargets, const int timeLimit, const int mutationRate)
{
	if(mutationRate > mAccuracy)
	{
		throw std::invalid_argument("The mutation Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(mutationRate));
	}
	std::minstd_rand0 popGen(std::chrono::system_clock::now().time_since_epoch().count());
	std::minstd_rand0 geneGen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<size_t> popDist(0,mPopulation.size()-1);


	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> shared_ptr<BuildList>
	{

		auto chooseIndividual = std::bind(popDist, popGen);
		BuildListGenerator<RacePolicy> buildListGen(techManager.getTechnologyList());
		buildListGen.initRandomGenerator();

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
				string entry = buildListGen.getRandomTechnologyName();
				newGenes.insert(newGenes.begin()+chooseGene(), entry);
			}
		} while(!(techManager.isBuildListPossible(newGenes)));

		return std::make_shared<BuildList>(newGenes);
	};


	const int nmutants = mPopulation.size() / 5;
	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	generateAndRate(target, fitnessPolicy, nmutants, genBuildList, timeLimit);
}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(const int selectionRate)
{
	if(selectionRate > mAccuracy)
	{
		throw std::invalid_argument("@BuildListOptimizer::select: The selection Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(selectionRate));
	}
	int threshold = (mPopulation.size()*selectionRate)/mAccuracy;
	if (threshold <= 0)
	{
		return;
	}
	mPopulation.erase(mPopulation.begin(), mPopulation.end()-threshold);
}

	template <class RacePolicy, class FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(const int accuracy, const int individualSize)
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
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(const string target, const int ntargets, const int timeLimit, const int initPopSize)
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
	else if(mPopulation.size() >= initPopSize)
	{
		std::sort(mPopulation.begin(), mPopulation.end());
		return;
	}

	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> shared_ptr<BuildList>
	{
		BuildListGenerator<RacePolicy> buildListGen(techManager.getTechnologyList());
		buildListGen.initRandomGenerator();
		return buildListGen.buildOneRandomList(mIndividualSize);
	};

	const int nindividuals = initPopSize-mPopulation.size();

	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	generateAndRate(target, fitnessPolicy, nindividuals, genBuildList, timeLimit);
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
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(const string target, const int ntargets, const int timeLimit, const int generations, const int reproductionRate, const int mutationRate, const int selectionRate)
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

	for(int i = 0; i < generations; ++i)
	{
		select(selectionRate);
		mutate(target, ntargets, timeLimit, mutationRate);
		crossover(target, ntargets, timeLimit, reproductionRate);
		sortPopulation();
	}

}


/* combined use of initialize and optimize */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clearInitializeAndOptimize(const string target, const int ntargets, const int timeLimit, const int initPopSize, const int generations, const int reproductionRate, const int mutationRate, const int selectionRate)
{
	clear();
	initialize(target, ntargets, timeLimit, initPopSize);
	optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);

}

/* combined use of initialize and optimize */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(const string target, const int ntargets, const int timeLimit, const int initPopSize, const int generations, const int reproductionRate, const int mutationRate, const int selectionRate)
{
	initialize(target, ntargets, timeLimit, initPopSize);
	optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);
}

/* adds a specific individual to the population */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::addIndividual(const string target, const int ntargets, const int timeLimit, shared_ptr<BuildList> buildList )
{
	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	map<int,const string> simRes = Simulation<RacePolicy>(buildList, mTechManager.getTechnologyList()).run(timeLimit);
	mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
				fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
}

/* get the group of size number of the fittest individuals, together with their corresponding fitness value */
	template <class RacePolicy, class FitnessPolicy>
vector<Individual> BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestGroup(const int groupSize)
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

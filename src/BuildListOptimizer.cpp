#include "../include/BuildListOptimizer.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::set;
using std::map;
using std::multimap;
using std::pair;
using std::future;
using std::async;


template<class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::generateAndRate(const string target, FitnessPolicy& fitnessPolicy, const size_t nindividuals, std::function<vector<string>(TechnologyManager<RacePolicy>)> genBuildList, const int timeLimit)
{
    const TechnologyList& techList = mTechManager.getTechnologyList();
    auto runSimulation = [=] (vector<string> dna) -> multimap<int, string>
    {
        shared_ptr<BuildList> buildList = std::make_shared<BuildList>(dna);
        Simulation<RacePolicy> a(buildList, techList);
		try
		{
        	return a.run(timeLimit);
		} catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			for(auto i : dna)
				std::cout << i << std::endl;
			return multimap<int,string>();
		}
    };

    auto rateIndividual = [=] (multimap<int, string> res, vector<string> dnaVec, FitnessPolicy fp) -> Individual
    {
        auto a = mTechManager.getEntityRequirements(target);
        Individual newOne(fp.rateBuildListHard(res), fp.rateBuildListSoft(res,RacePolicy::getWorker(), a),dnaVec);
		return newOne;
	};
	size_t availableThreads = std::thread::hardware_concurrency();
	availableThreads++;
	const unsigned int NUM_THREADS = std::min(availableThreads+1,nindividuals);
	vector<size_t>threadID(NUM_THREADS);
	vector<future<vector<string>>> dnaFutureVec(NUM_THREADS);
    vector<future<multimap<int, string>>> resultFutureVec(NUM_THREADS);
	vector<future<Individual>> individualFutureVec(NUM_THREADS);
	vector<vector<string>> dnaVec(nindividuals);
    vector<multimap<int, string>> simRes(nindividuals);

#ifdef DEBUG
	int threadFailures = 0;
#endif

	size_t started=0;
	size_t finished=0;
	std::future_status status;

    for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		dnaFutureVec[i] = async(std::launch::async,genBuildList, mTechManager);
		threadID[i] = started;
		++started;
	}
	while (finished < nindividuals)
	{
        for (size_t i = 0; i < NUM_THREADS; ++i)
		{
			try
			{
				//watch status
				try
				{
					status = dnaFutureVec[i].wait_for(std::chrono::milliseconds(200));
				} catch (std::future_error &e)
				{
					//only happens if no thread associated with future-object
					continue;
				}
				if (status == std::future_status::ready) {
					dnaVec.at(finished) = dnaFutureVec[i].get(); 
					++finished;
					if (started < nindividuals)
					{
						++started;
						threadID[i] = started;
						dnaFutureVec[i] = async(std::launch::async,genBuildList, mTechManager);
					}
				} else
				{
					std::cout << "Buildlist-Thread not yet ready " << threadID[i] << std::endl;
				}
			} catch(std::system_error&)
			{
				dnaVec.at(finished++) = genBuildList(mTechManager);
			}
		}
	}
	std::cout << "New buildlists finished" << std::endl;

	finished = started = 0;
    for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		resultFutureVec[i] = async(std::launch::async,runSimulation,dnaVec.at(started));
		threadID[i] = started;
		++started;
	}

	while (finished < nindividuals)
	{
        for (size_t i = 0; i < NUM_THREADS; ++i)
		{
			try
			{
				//watch status
				try
				{
					status = resultFutureVec[i].wait_for(std::chrono::milliseconds(200));
				} catch (std::future_error &e)
				{
					//only happens if no thread associated with future-object
					continue;
				}
				if (status == std::future_status::ready) {
					simRes[finished] = resultFutureVec[i].get();
					++finished;
					if (started < nindividuals)
					{
						threadID[i] = started;
						resultFutureVec[i] = async(std::launch::async,runSimulation,dnaVec.at(started++));
					}
				} else
				{
					std::cout << "Simulation not yet ready " << threadID[i] << std::endl;
				}
			} catch (std::system_error &e)
			{
				simRes[finished++] = runSimulation(dnaVec.at(started++));
				std::cerr << "Thread didnt start properly " << e.what() << std::endl;
			}
		}
	}
	std::cout << "Simulations done" << std::endl;

	finished = started = 0;
    for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		individualFutureVec[i] = async(std::launch::async,rateIndividual,simRes[started],dnaVec.at(started), fitnessPolicy);
		threadID[i] = started;
		++started;
	}
	while (finished < nindividuals)
	{
        for (size_t i = 0; i < NUM_THREADS; ++i)
		{
			try
			{
				//watch status
				try
				{
					status = individualFutureVec[i].wait_for(std::chrono::milliseconds(200));
				} catch (std::future_error &e)
				{
					//only happens if no thread associated with future-object
					continue;
				}
				if (status == std::future_status::ready) {
					++finished;
					mPopulation.push_back(individualFutureVec[i].get());
					if (started < nindividuals)
					{
						threadID[i] = started;
						individualFutureVec[i] = async(std::launch::async,rateIndividual,simRes[started],dnaVec.at(started), fitnessPolicy);
						++started;
					}
				} else
				{
					std::cout << "Individual-Creation not yet ready " << threadID[i] << std::endl;
				}
			} catch(std::system_error &e)
			{
				mPopulation.push_back(rateIndividual(simRes[i],dnaVec[i], fitnessPolicy));
				++finished;
				std::cerr << "Thread didnt start properly " << e.what() << std::endl;
			}
		}
	}
	std::cout << "Individuals created" << std::endl;
}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::crossover(const string target, const size_t ntargets, const int timeLimit, const size_t reproductionRate)
{
	if(reproductionRate > mAccuracy)
	{
		throw std::invalid_argument("The reproduction Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(reproductionRate));
	}
	std::minstd_rand0 generator1(std::chrono::system_clock::now().time_since_epoch().count());
	std::minstd_rand0 generator2(std::chrono::system_clock::now().time_since_epoch().count());


	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> vector<string>
	{

		std::uniform_int_distribution<size_t> distribution1(0,mPopulation.size()-1);
		std::uniform_int_distribution<int> distribution2(0,1);


		auto chooseIndividual = std::bind(distribution1, generator1);
		auto coin =  std::bind(distribution2, generator2);

		std::hash<string> hashGen;
		set<size_t> positions;
		vector<string> newGenes;
		size_t count = 0;
		do {
			if(++count > 10000)
			{
                BuildListGenerator<RacePolicy> buildListGen(techManager.getTechnologyList());
				buildListGen.initRandomGenerator(target, 8);
                return buildListGen.buildOneRandomList(mIndividualSize)->getAsVector();
			}

			PROGRESS("Try to create valid child");
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

						size_t res=0;
						size_t len = ind1.genes.size() < ind2.genes.size() ? ind1.genes.size() : ind2.genes.size();
						for(size_t k = 0; k < len; ++k)
						{
							res += (len-k) * std::abs(hashGen(ind1.genes[k])-hashGen(ind2.genes[k]));
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
		return newGenes;
	};

	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	const size_t ncrossovers = mPopulation.size()*reproductionRate / mAccuracy;
	generateAndRate(target, fitnessPolicy, ncrossovers, genBuildList, timeLimit);

}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::mutate(const string target, const size_t ntargets, const int timeLimit, const size_t mutationRate)
{
	if(mutationRate > mAccuracy)
	{
		throw std::invalid_argument("The mutation Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(mutationRate));
	}

	std::minstd_rand0 popGen(std::chrono::system_clock::now().time_since_epoch().count());
	std::minstd_rand0 geneGen(std::chrono::system_clock::now().time_since_epoch().count());

	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> vector<string>
	{

		std::uniform_int_distribution<size_t> popDist(0,mPopulation.size()-1);

		auto chooseIndividual = std::bind(popDist, popGen);
		BuildListGenerator<RacePolicy> buildListGen(techManager.getTechnologyList());
		buildListGen.initRandomGenerator(target, 8);

		vector<string> newGenes;
		size_t count = 0;
		do
		{
			PROGRESS("Try to create valid mutant");
			const Individual& oldInd = mPopulation[chooseIndividual()];
			if(++count > 10000)
			{
                return buildListGen.buildOneRandomList(mIndividualSize)->getAsVector();
			}
			newGenes = oldInd.genes;
			std::uniform_int_distribution<size_t> geneDist(0,newGenes.size()-1);
			auto chooseGene = std::bind(geneDist, geneGen);
			for(size_t i = 0; i < (mutationRate * newGenes.size())/mAccuracy; ++i)
			{

				geneDist = std::uniform_int_distribution<size_t>(0,newGenes.size()-1);
				newGenes.erase(newGenes.begin()+chooseGene());

				geneDist = std::uniform_int_distribution<size_t>(0,newGenes.size()-1);
				string entry = buildListGen.getRandomTechnologyName();
				newGenes.insert(newGenes.begin()+chooseGene(), entry);
			}
		} while(!(techManager.isBuildListPossible(newGenes)));

		return newGenes;
	};


	const size_t nmutants = mPopulation.size() / 4;
	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
	generateAndRate(target, fitnessPolicy, nmutants, genBuildList, timeLimit);
}

	template <class RacePolicy, class FitnessPolicy>
inline void BuildListOptimizer<RacePolicy, FitnessPolicy>::select(const size_t selectionRate)
{
	if(selectionRate > mAccuracy)
	{
		throw std::invalid_argument("@BuildListOptimizer::select: The selection Rate must be lower or equal the maximum value. The passed value is: "+std::to_string(selectionRate));
	}
	size_t threshold = (mPopulation.size()*selectionRate)/mAccuracy;
	if (threshold <= 1)
	{
		return;
	}
	mPopulation.erase(mPopulation.begin(), mPopulation.end()-threshold);
}

	template <class RacePolicy, class FitnessPolicy>
BuildListOptimizer<RacePolicy, FitnessPolicy>::BuildListOptimizer(const size_t accuracy, const size_t individualSize)
{
	if(accuracy <= 0)
	{
		throw std::invalid_argument("@BuildListOptimizer::BuildListOptimizer(size_t accuracy, size_t individualSize): The accuracy must be greater than zero. The value passed is: "+std::to_string(accuracy));
	}
	if(individualSize <= 0)
	{
		throw std::invalid_argument("@BuildListOptimizer::BuildListOptimizer(size_t accuracy, size_t individualSize): The size of the individuals must be greater than zero. The value passed is: "+std::to_string(accuracy));
	}

	mAccuracy = accuracy;
	mIndividualSize = individualSize;

	mTechManager = TechnologyManager<RacePolicy>();
}


/*initializes the population with random individuals until the population size reaches initPopSize*/
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initialize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize)
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

	auto genBuildList = [=] (TechnologyManager<RacePolicy> techManager) -> vector<string>
	{
		BuildListGenerator<RacePolicy> buildListGen(techManager.getTechnologyList());
		buildListGen.initRandomGenerator(target, 8);
		return buildListGen.buildOneRandomList(mIndividualSize)->getAsVector();
	};

	const size_t nindividuals = initPopSize-mPopulation.size();

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
void BuildListOptimizer<RacePolicy, FitnessPolicy>::optimize(const string target, const size_t ntargets, const int timeLimit, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate)
{

	size_t variableSelect = selectionRate; 
	int subtrahend=1;
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

	for(size_t i = 0; i < generations; ++i)
	{
		select(variableSelect);
		mutate(target, ntargets, timeLimit, mutationRate);
		crossover(target, ntargets, timeLimit, reproductionRate);
		sortPopulation();
		//if subtrahend equals 0, select + mutate + crossover will result in old vector size
		if (mPopulation.size()<100)
			subtrahend=0;
		variableSelect = std::min((unsigned int)(mAccuracy*mAccuracy/((1.25*mAccuracy*(mAccuracy+reproductionRate))/mAccuracy)-subtrahend), ((unsigned int)(variableSelect + (mAccuracy-variableSelect)/6)));
		std::cout << "Size of the population after " << std::to_string(i+1) << " generation(s): " << std::to_string(mPopulation.size()) << "\t select: " << variableSelect << std::endl;
	}
}


/* combined use of initialize and optimize */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::clearInitializeAndOptimize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate)
{
	clear();
	initialize(target, ntargets, timeLimit, initPopSize);
	optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);

}

/* combined use of initialize and optimize */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::initializeAndOptimize(const string target, const size_t ntargets, const int timeLimit, const size_t initPopSize, const size_t generations, const size_t reproductionRate, const size_t mutationRate, const size_t selectionRate)
{
	initialize(target, ntargets, timeLimit, initPopSize);
	optimize(target, ntargets, timeLimit, generations, reproductionRate, mutationRate, selectionRate);
}

/* adds a specific individual to the population */
	template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::addIndividual(const string target, const size_t ntargets, const int timeLimit, shared_ptr<BuildList> buildList )
{
	FitnessPolicy fitnessPolicy(target, timeLimit, ntargets);
    multimap<int, string> simRes = Simulation<RacePolicy>(buildList, mTechManager.getTechnologyList()).run(timeLimit);
	mPopulation.push_back(Individual(fitnessPolicy.rateBuildListHard(simRes),
				fitnessPolicy.rateBuildListSoft(simRes, RacePolicy::getWorker(), mTechManager.getEntityRequirements(target)), buildList->getAsVector()));
}

/* get the group of size number of the fittest individuals, together with their corresponding fitness value */
	template <class RacePolicy, class FitnessPolicy>
vector<Individual> BuildListOptimizer<RacePolicy, FitnessPolicy>::getFittestGroup(const size_t groupSize)
{

	if (mPopulation.size() < groupSize)
	{
		//throw std::invalid_argument("@BuildListOptimizer::getFittestGroup: Lower population than requested groupSize. The value passed is: "+std::to_string(groupSize));
		std::cerr << "@BuildListOptimizer::getFittestGroup: Lower population than requested groupSize. The value passed is: " << groupSize << std::endl;
		vector<Individual> res;
		res.push_back(getFittestIndividual());
		return res;
	}

	sortPopulation();
	vector<Individual> res;
	for (size_t i = mPopulation.size()-1; i >= mPopulation.size()-groupSize; --i)
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

template <class RacePolicy, class FitnessPolicy>
void BuildListOptimizer<RacePolicy, FitnessPolicy>::printBest(int timeLimit, std::string target)
{
    TechnologyList techList = mTechManager.getTechnologyList();
	auto best = getFittestGroup(1).at(0);
	auto bList = std::make_shared<BuildList>(best.genes);
	size_t counter=0;
	size_t targetCount=0;
	std::multimap<int,std::string> finishOrder;

    Simulation<RacePolicy> sim(bList, techList);
    std::multimap<int, string> simRes = sim.run(timeLimit);
	std::vector<std::string> output;

	for (auto it : simRes)
	{
		if (it.second.compare("Time")==0)
			break;
		std::stringstream sstream;
		sstream << std::setw(2) << ++counter << ":" << std::setw(4) << it.first << "" << std::setw(21) << it.second;
		std::string tmp = sstream.str();
		output.push_back(tmp);
		tmp = it.second;
		auto tech = techList.findTechnology(tmp);
		std::pair<int,std::string> in(it.first+tech->getBuildTime(),it.second);
		finishOrder.insert(in);

		if (target.compare(it.second) == 0)
			++targetCount;
		if ((target.compare(it.second) == 0) && (FitnessPolicy::getFitnessType() == FitnessType::Push))
			break;
	}
	counter=0;
	for (auto it : finishOrder)
	{
		std::stringstream sstream;
		sstream << "\t\t" << std::setw(4) << std::to_string(it.first) << std::setw(21) << it.second;
		std::string tmp = sstream.str();

		output.at(counter) += tmp;
		++counter;
	}
	std::cout << "sorted by: " << std::endl;
	std::cout << std::setw(22) << "starting time:" << "\t\t" << std::setw(22) << "finished time:" << std::endl;
	for (size_t i = 0; i < output.size(); ++i)
	{
		std::cout << output.at(i) << std::endl;
	}
	std::cout << "We produced " << targetCount << " " << target << "s" << std::endl;
}

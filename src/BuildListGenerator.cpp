#include "BuildListGenerator.h"

//do never ever ever call this function, its only for the compiler
//This way we force the compiler to evaluate Templates!
inline void funny()
{
	BuildListGenerator<Protoss> bla();
	BuildListGenerator<Terran> blab();
	BuildListGenerator<Zerg> blabla();
}

template <class RacePolicy>
BuildListGenerator<RacePolicy>::BuildListGenerator()
{
	if (!(InitTechTree<RacePolicy>(&copyFrom).initTechTree()))
	{
		throw std::runtime_error("TechnologyList initialization failed.");
	}
	copyFrom.reset();
	localTester = new TechnologyManager<RacePolicy>(this->copyFrom);
}
template <class RacePolicy>
BuildListGenerator<RacePolicy>::BuildListGenerator(const TechnologyList &techList)
{
	this->copyFrom = techList;
	copyFrom.reset();
	localTester = new TechnologyManager<RacePolicy>(this->copyFrom);
}

template <class RacePolicy>
bool BuildListGenerator<RacePolicy>::checkBuildListPossibility(std::vector<std::string> buildList)
{
	bool check;
	try
	{
		check = localTester->isBuildListPossible(buildList);
	} catch (std::exception &e)
	{
		return false;
	}
	return check;
}

template <class RacePolicy>
void BuildListGenerator<RacePolicy>::initRandomGenerator(std::string SpecialOne, int weight)
{
	size_t tmp = std::chrono::system_clock::now().time_since_epoch().count();
    //std::cout << "For Debug Purpose: BuildListGenerator-Seed: " << tmp << std::endl;
    copyFrom.initRandomGenerator(tmp,SpecialOne,weight);
}

template <class RacePolicy>
std::string BuildListGenerator<RacePolicy>::getRandomTechnologyName()
{
        return copyFrom.getRandomTechnology();
}

template <class RacePolicy>
std::shared_ptr<BuildList> BuildListGenerator<RacePolicy>::buildOneRandomList(int length)
{
	std::vector<std::string> stringList;
	std::shared_ptr<BuildList> list;
	do
	{
		localTester = new TechnologyManager<RacePolicy>(this->copyFrom);
		stringList.clear();
		int supply = 4;
		for (int i = 0; i < length; ++i)
		{
			std::string in;
			bool checked = false;
			do
			{
				in = copyFrom.getRandomTechnology();
				std::vector<std::shared_ptr<Technology>> tmp = localTester->findTechnology(in);
				for (unsigned int j=0;j<tmp.size();++j)
				{
					checked=localTester->checkTechnologyRequirements(tmp[j]);
					if (checked == true)
					{
						if(!in.compare(RacePolicy::getMainBuilding()))
						{   
							supply += 10; 
						}   
						else if(!in.compare(RacePolicy::getSupplyProvider()))
						{   
							supply += 8;
						} else
						{   
							supply -= tmp[j]->getSupplyCost();
							if (supply < 0)
							{   
								checked = false;
							} else
							{
								checked = true;
								break;
							}
						} 
					}
					break;
				}
			} while (checked == false);
			localTester->notifyCreation(in);
			stringList.push_back(in);
		}
		//shouldnt reach this one
		if (checkBuildListPossibility(stringList)==false)
		{
			std::cerr << "Nicht gut, kaputte RandomList" << std::endl;
			for (unsigned int j=0;j<stringList.size();++j)
				std::cout << stringList.at(j) << std::endl;
		}
	} while (checkBuildListPossibility(stringList) == false);
	list = std::make_shared<BuildList>(stringList);

	return list;
}

	template <class RacePolicy>
std::vector<std::shared_ptr<BuildList>> BuildListGenerator<RacePolicy>::buildManyRandomLists(int population, int length)
{
	std::vector<std::shared_ptr<BuildList>> buildlistVector;
	for (int i = 0; i < population; ++i)
	{
		buildlistVector.push_back(buildOneRandomList(length));
		if (i%1000==0)
			std::cout << i << "\t done" << std::endl;
	}
	return buildlistVector;
}


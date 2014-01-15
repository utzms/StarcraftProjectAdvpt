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
	localTester = new TechnologyManager<RacePolicy>(&this->copyFrom);
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
	copyFrom.initRandomGenerator(2221,SpecialOne,weight);
}

template <class RacePolicy>
std::shared_ptr<BuildList> BuildListGenerator<RacePolicy>::buildOneRandomList(int length)
{
	std::vector<std::string> stringList;
	std::shared_ptr<BuildList> list;
	do
	{
		stringList.clear();
		for (int i = 0; i < length; ++i)
		{
			stringList.push_back(copyFrom.getRandomTechnology());
		//	std::cout << stringList[i] << std::endl;
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
		std::cout << i << "\t done" << std::endl;
	}
	return buildlistVector;
}


#include "../include/BuildListGenerator.h"

//do never ever ever call this function, its only for the compiler
//This way we force the compiler to evaluate Templates!


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
#ifdef DEBUG
    std::cerr << "For Debug Purpose: BuildListGenerator-Seed: " << tmp << std::endl;
#endif
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
    PROGRESS("BuildListGenerator::buildOneRandomList()");
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
				if (in == "Larva")
					continue;
				std::vector<std::string> killed;
				bool fulfilled = false;
				std::shared_ptr<Technology> itseMe = tmp.at(0);
				for (unsigned int j=0;j<tmp.size();++j)
				{
					//checked=localTester->checkTechnologyRequirements(tmp[j]);
					std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> requirements = tmp[j]->getRequirements();
					itseMe = tmp[j];

					//std::cout << "\n" << in << " " << requirements.size() << std::endl;
					for(auto redundantRequirements : requirements)
					{
						//std::cout << "\t" << redundantRequirements.size() << std::endl;
						fulfilled = false;
						for(auto requirement : redundantRequirements)
						{ 
							std::string name = requirement.first->getName();
						//	std::cout << "\t\t" << name;
							if (requirement.second == RequirementType::Vanishing)
							{
								if (localTester->entityExists(name))
								{
									killed.push_back(name);
									localTester->notifyDestruction(name);
									fulfilled = true;
								} else
								{
						//			std::cout << "\t not" <<std::endl;
									fulfilled = false;
									break;
								}
							} else if (localTester->entityExists(name))
							{
								fulfilled = true;
								break;
							} else
							{
						//		std::cout << "\t not yet" << std::endl;
							}
						}
						if(!fulfilled)
						{
							for (auto it : killed)
							{
								localTester->notifyCreation(it);
							}
							break;
						}
					}
					if (fulfilled == true)
					{
					//	std::cout << "\t\t OK" << std::endl;
						break;
					}
				}
				//if (fulfilled==false)
				//	std::cout << "\t\t NO" << std::endl;
				checked = fulfilled;
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
						supply -= itseMe->getSupplyCost();
						if (supply < 0)
						{   
							for (auto it : killed)
							{
								localTester->notifyCreation(it);
							}
							killed.clear();
							checked = false;
						} else
						{
							checked = true;
							break;
						}
					}
				}
			} while (checked == false);
			/*
			   std::pair<bool, std::vector<std::string> > res;
			   std::vector<std::shared_ptr<Technology>> techVec = localTester->findTechnology(in);
			   localTester->checkAndGetVanishing(techVec,res);
			   if(res.first == false)
			   {
			   throw std::runtime_error("@BuildListGenerator::buildOneRandomList: checkTechnologyRequirements returned true, but checkAndGetVanishing() false for the same entity name, something went terribly wrong.");

			   }
			   for(std::string name : res.second)
			   {
			   localTester->notifyDestruction(name);
			   }
			 */
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


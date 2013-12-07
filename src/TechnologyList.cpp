#include <TechnologyList.h>

TechnologyList::TechnologyList()
{
}

void TechnologyList::initUnitList(std::string unitList)
{
	DataReader readFile(unitList);
	std::string line;
	while (!((line=readFile.getLine()).empty()))
	{
		std::string unitName="";
		float unitCostMinerals=0.0f;
		float unitCostGas=0.0f;
		float unitCostSupply=0.0f;
		int unitBuildTime=0;
		std::string requirements;
		std::stringstream stream;

		stream << line;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitName;
		if (unitName == "Unit")
			continue;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostMinerals;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostGas;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostSupply;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitBuildTime;

		Technology tech();
		units.insert( std::pair<std::string, std::shared_ptr<Technology>>(unitName,
					std::shared_ptr<Technology>(new Technology(unitName,unitCostMinerals,unitCostGas,unitCostSupply,unitBuildTime))));
	}
}

void TechnologyList::initBuildingList(std::string buildList)
{
	DataReader readFile(buildList);
	std::string line;
	while (!((line=readFile.getLine()).empty()))
	{
		std::string buildName="";
		float buildCostMinerals=0.0f;
		float buildCostGas=0.0f;
		float buildCostSupply=0.0f;
		int buildBuildTime=0;
		std::string requirements;
		std::stringstream stream;

		stream << line;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildName;
		if (buildName == "Building")
		{
			continue;
		}

		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostMinerals;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostGas;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildBuildTime;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostSupply;

		Technology *t = new Technology(buildName,buildCostMinerals,buildCostGas,buildCostSupply,buildBuildTime);
		std::shared_ptr<Technology> tech(t);
		buildings.insert( std::pair<std::string, std::shared_ptr<Technology>>(buildName,
					tech));


		//requirements
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> requirements;
		size_t from=0;
		size_t to=0;
		std::string sub;
		std::shared_ptr<Technology> tmp;
		while ((to=requirements.find(",")) != std::string::npos)
		{
			sub = requirements.substr(from,to);
			requirements = requirements.substr(to+1,std::string::npos);
			tmp = findBuilding(sub);
			if (tmp == NULL)
			{
#ifdef DEBUG
				std::cerr << "\t must be an unit" << std::endl; 
#endif
				tmp = findUnit(sub);
				if (tmp == NULL)
				{
#ifdef DEBUG
					std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
					unresolvedBuildingRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
								tech));
					continue;
				}
			}
			tech->addRequirement(tmp);
		}
		sub = requirements.substr(from,std::string::npos);
		if (!(sub == "0"))
		{
			tmp = findBuilding(sub);
			if (tmp == NULL)
			{
#ifdef DEBUG
				std::cerr << "\t must be an unit" << std::endl; 
#endif
				tmp = findUnit(sub);
				if (tmp == NULL)
				{
#ifdef DEBUG
					std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
					unresolvedBuildingRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
								tech));
				} else
				{
					tech->addRequirement(tmp);
				}
			} else
			{
				tech->addRequirement(tmp);
			}
		}
	}
}


std::shared_ptr<Technology> TechnologyList::findBuilding(std::string key)
{
	static int currentTech=0;
	static std::string lastKey="";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	auto count = buildings.count(key);
	if (count == 0)
	{
		std::cerr << "Nothing found in building list with key:\t" << key << std::endl;
		return NULL;
	} else
	{
		if (lastKey == key)
		{
			++it;
			if (it != buildings.equal_range(key).second)
			{
				return (*it).second;
			} else
			{
#ifdef DEBUG
				std::cout << "No other building with key:\t" << key << " found" << std::endl;
#endif
				return NULL;
			}
		} else
		{
			lastKey=key;
			it = buildings.equal_range(key).first;
			return (*it).second;
		}
	}
}

std::shared_ptr<Technology> TechnologyList::findUnit(std::string key)
{
	static int currentTech=0;
	static std::string lastKey="PlaceHolder";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = units.count(key);
	std::cout << count << std::endl;
	if (count == 0)
	{
		lastKey=key;
		std::cerr << "Nothing found in unit list with key:\t" << key << std::endl;
		return NULL;
	} else
	{
		if (lastKey == key)
		{
			++it;
			if (it != units.equal_range(key).second)
			{
				return (*it).second;
			} else
			{
#ifdef DEBUG
				std::cout << "No other unit with key:\t" << key << " found" << std::endl;
#endif
				return NULL;
			}
		} else
		{
			lastKey=key;
			it = units.equal_range(key).first;
			return (*it).second;
		}
	}
}


#include <TechnologyList.h>

TechnologyList::TechnologyList()
{
	initialized=false;
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

		Technology *t = new Technology(unitName,unitCostMinerals,unitCostGas,unitCostSupply,unitBuildTime);
		std::shared_ptr<Technology> tech(t);
		initialized=true;

		units.insert( std::pair<std::string, std::shared_ptr<Technology>>(unitName,
					tech));

		//resolve unresolved buildigns if needed
		auto it = unresolvedBuildingRequirements.equal_range(unitName).first;
		for(;it!=unresolvedBuildingRequirements.equal_range(unitName).second;++it)
		{
			(*it).second->addRequirement(tech);
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
			bool found=false;
			while((tmp=findUnit(sub))!=NULL)
			{
				found=true;
				tech->addRequirement(tmp);
			}
#ifdef DEBUG
			std::cerr << "\t must be a building" << std::endl; 
#endif
			if (!found)
			{
				while((tmp=findBuilding(sub))!=NULL)
				{
					found=true;
					tech->addRequirement(tmp);
				}
			}
			if (!found)
			{
#ifdef DEBUG
				std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
				unresolvedUnitRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
							tech));
				continue;
			}
		}
		sub = requirements.substr(from,std::string::npos);
		if (!(sub == "0"))
		{
			bool found=false;
			while((tmp=findUnit(sub))!=NULL)
			{
				found=true;
				tech->addRequirement(tmp);
			}
#ifdef DEBUG
			std::cerr << "\t must be an unit" << std::endl; 
#endif
			if (!found)
			{
				while((tmp=findBuilding(sub))!=NULL)
				{
					found=true;
					tech->addRequirement(tmp);
				}
			}
			if (!found)
			{
#ifdef DEBUG
				std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
				unresolvedUnitRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
							tech));
				continue;
			}
		}
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
		initialized=true;

		//resolve unresolved units if needed
		auto it = unresolvedUnitRequirements.equal_range(buildName).first;
		for(;it!=unresolvedUnitRequirements.equal_range(buildName).second;++it)
		{
			(*it).second->addRequirement(tech);
		}


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
			bool found=false;
			while((tmp=findBuilding(sub))!=NULL)
			{
				found=true;
				tech->addRequirement(tmp);
			}
#ifdef DEBUG
			std::cerr << "\t must be an unit" << std::endl; 
#endif
			if (!found)
			{
				while((tmp=findUnit(sub))!=NULL)
				{
					found=true;
					tech->addRequirement(tmp);
				}
			}
			if (!found)
			{
#ifdef DEBUG
				std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
				unresolvedBuildingRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
							tech));
				continue;
			}
		}
		sub = requirements.substr(from,std::string::npos);
		if (!(sub == "0"))
		{
			bool found=false;
			while((tmp=findBuilding(sub))!=NULL)
			{
				found=true;
				tech->addRequirement(tmp);
			}
#ifdef DEBUG
			std::cerr << "\t must be an unit" << std::endl; 
#endif
			if (!found)
			{
				while((tmp=findUnit(sub))!=NULL)
				{
					found=true;
					tech->addRequirement(tmp);
				}
			}
			if (!found)
			{
#ifdef DEBUG
				std::cerr << "\t couldnt be resolved, unit list not built yet, will be resolved back then" << std::endl;
#endif
				unresolvedBuildingRequirements.insert( std::pair<std::string,std::shared_ptr<Technology>>(sub,
							tech));
				continue;
			}
		}
	}
}

bool TechnologyList::isInitialized()
{
	return initialized;
}

std::shared_ptr<Technology> TechnologyList::findBuilding(std::string key)
{
	static std::string lastKey="PlaceHolder";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = buildings.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG
		std::cerr << "Nothing found in building list with key:\t" << key << std::endl;
#endif
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
				lastKey = "PlaceHolder";
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
	static std::string lastKey="PlaceHolder";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = units.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG
		std::cerr << "Nothing found in unit list with key:\t" << key << std::endl;
#endif
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
				lastKey = "PlaceHolder";
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


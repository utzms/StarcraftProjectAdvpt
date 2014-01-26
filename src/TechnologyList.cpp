#include "../include/TechnologyList.h"

TechnologyList::TechnologyList()
{
	initialized=false;

}
TechnologyList::TechnologyList(std::string unitPathIn, std::string buildPath)
{
	initialized=false;

	initUnitList(unitPathIn);
    initBuildingList(buildPath);
    initTechnologySet();
}

TechnologyList::~TechnologyList()
{
	PROGRESS("TL Destructor");
	cleanAll();
}

TechnologyList::TechnologyList(TechnologyList &other)
{
	PROGRESS("TL CopyConstructor");
	if (&other != this)
	{
		this->units = other.units;
		this->buildings = other.buildings;
		this->initialized = other.initialized;
		this->unitPath = other.unitPath;
		this->buildingPath = other.buildingPath;
		this->unresolvedBuildingRequirements = other.unresolvedBuildingRequirements;
        this->techSet = other.techSet;

        /* no longer required
		for (auto &it : this->units)
		{   
			it.second->setExistence(0);
		}   
		for (auto &it : this->buildings)
		{   
			it.second->setExistence(0);
        }
        */
	}
}

TechnologyList::TechnologyList(const TechnologyList &other)
{
	PROGRESS("TL CopyConstructor");
	if (&other != this)
	{
		this->units = other.units;
		this->buildings = other.buildings;
		this->initialized = other.initialized;
		this->unitPath = other.unitPath;
		this->buildingPath = other.buildingPath;
        this->unresolvedBuildingRequirements = other.unresolvedBuildingRequirements;
        this->techSet = other.techSet;

        /* no longer required
		for (auto &it : this->units)
		{   
			it.second->setExistence(0);
		}   
		for (auto &it : this->buildings)
		{   
			it.second->setExistence(0);
        }
        */
	}
}

void TechnologyList::cleanAll()
{
	cleanUnresolved();
	if (!(units.size() == 0))
		units.clear();
	if (!(buildings.size() == 0))
        buildings.clear();
}

void TechnologyList::cleanUnresolved()
{
	if (!(unresolvedBuildingRequirements.empty()))
		unresolvedBuildingRequirements.clear();
	if (!(unresolvedUnitRequirements.empty()))
		unresolvedUnitRequirements.clear();
}

void TechnologyList::linkRequirement(std::shared_ptr<Technology> &tech, std::vector<std::vector<std::string>> in, RequirementType T)
{
	for (size_t j = 0; j < in.size(); ++j)
	{
		std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
		for (size_t i = 0; i < in[j].size(); ++i)
		{
			std::shared_ptr<Technology> requirement;
			std::pair<std::shared_ptr<Technology>,RequirementType> commit;
			commit.second=T;
			if ((requirement=findTechnology(in[j][i]))==NULL)
			{
				std::cerr << "Fatal error: No instance of Technology: " << in[j][i] << std::endl;
				return;
			}
			commit.first = requirement;
			sources.push_back(commit);
		}
		tech->addRequirement(sources);
	}
}

void TechnologyList::linkRequirement(std::shared_ptr<Technology> &tech, std::vector<std::string> in, RequirementType T)
{
	std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
	for (size_t i = 0; i < in.size(); ++i)
	{
		std::shared_ptr<Technology> requirement;
		std::pair<std::shared_ptr<Technology>,RequirementType> commit;
		commit.second=T;
		if ((requirement=findTechnology(in[i]))==NULL)
		{
			std::cerr << "Fatal error: No instance of Technology: " << in[i] << std::endl;
			return;
		}
		commit.first = requirement;
		sources.push_back(commit);
	}
	tech->addRequirement(sources);
}

void TechnologyList::tokenizeString(std::vector<std::string> &out, std::string in, char sep)
{
	size_t from=0,to=0;
	std::string sub;
	do 
	{
		to = in.find(sep);
		sub = in.substr(from,to);
		if (sub.compare("0") != 0)
			out.push_back(sub);
		in=in.substr(to+1,std::string::npos);
	} while (to!=std::string::npos);
}

void TechnologyList::initRest()
{
#ifdef DEBUG_DEEP
	std::cout << "INIT REST of TechnolgyList" << std::endl;
#endif
	std::string requirements;
	std::string vanish;
	std::string buildFrom;

	auto unitIt = unresolvedUnitRequirements.begin();
	for (; unitIt != unresolvedUnitRequirements.end(); ++unitIt)
	{
		std::shared_ptr<Technology> tech = unitIt->first;
		requirements = unitIt->second.req;
		vanish = unitIt->second.vanish;
		buildFrom = unitIt->second.buildFrom;
		/*
		 * Requirement Check:
		 */
		std::vector<std::string> require;
		tokenizeString(require, requirements, ',');
		std::vector<std::vector<std::string>> oneOfMany;
		for (size_t i = 0; i < require.size(); ++i)
		{
			std::vector<std::string> tmp;
			tokenizeString(tmp, require[i], '/');
			if (!(tmp.empty()))
			{
				oneOfMany.push_back(tmp);
			}
		}
		linkRequirement(tech, oneOfMany, RequirementType::Existence);
		/*
		 * Build From Check:
		 */
		std::vector<std::string> build;
		tokenizeString(build, buildFrom, ',');
		linkRequirement(tech, build, RequirementType::ForProduction);
		/*
		 * Vanish Check:
		 */
		std::vector<std::string> van;
		tokenizeString(van, vanish, ',');
		linkRequirement(tech, van, RequirementType::Vanishing);
	} //end unit it for loop

	//here starts the building list
	auto buildIt = unresolvedBuildingRequirements.begin();
	for (; buildIt != unresolvedBuildingRequirements.end(); ++buildIt)
	{
		std::shared_ptr<Technology> tech = buildIt->first;
		requirements = buildIt->second.req;
		vanish = buildIt->second.vanish;
		/*
		 * Requirement Check:
		 */
		std::vector<std::string> require;
		tokenizeString(require, requirements, ',');
		std::vector<std::vector<std::string>> oneOfMany;
		for (size_t i = 0; i < require.size(); ++i)
		{
			std::vector<std::string> tmp;
			tokenizeString(tmp, require[i], '/');
			if (!(tmp.empty()))
			{
				oneOfMany.push_back(tmp);
			}
		}
		linkRequirement(tech, oneOfMany, RequirementType::Existence);
		/*
		 * Vanish Check:
		 */
		std::vector<std::string> van;
		tokenizeString(van, vanish, ',');
		linkRequirement(tech, van, RequirementType::Vanishing);
	}
	PROGRESS("TL Init done")
		cleanUnresolved();
}

	template <typename T>
inline bool TechnologyList::streamToVariable(std::stringstream &stream, T &x)
{
	if (!(stream.good()))
	{   
		//std::cerr << "TechnologyList::streamToVariable: stream not good, probably no valid line"<<std::endl;
		return false;
	}
	stream >> x;
	return true;
}

void TechnologyList::initUnitList(std::string unitList)
{
	this->unitPath=unitList;
	DataReader readFile(unitList);
	std::string line;
	bool callRest=initialized;
	while (!((line=readFile.getLine()).empty()))
	{
		initialized=true;
		std::string unitName="";
		float unitCostMinerals=0.0f;
		float unitCostGas=0.0f;
		float unitCostSupply=0.0f;
		int unitBuildTime=0;
		std::string requirements;
		std::string buildFrom;
		std::string vanish;
		std::stringstream stream;

		stream << line;
		if (!(streamToVariable(stream, unitName)))
			continue;
		if (!(streamToVariable(stream, unitCostMinerals)))
			continue;
		if (!(streamToVariable(stream, unitCostGas)))
			continue;
		if (!(streamToVariable(stream, unitCostSupply)))
			continue;
		if (!(streamToVariable(stream, unitBuildTime)))
			continue;
		if (!(streamToVariable(stream, buildFrom)))
			continue;
		if (!(streamToVariable(stream, requirements)))
			continue;
		if (!(streamToVariable(stream, vanish)))
			continue;
		if (unitName == "Unit")
			continue;

		std::shared_ptr<Technology> tech = std::make_shared<Technology>(unitName,unitCostMinerals,unitCostGas,unitCostSupply,unitBuildTime);

		units.insert( std::pair<std::string, std::shared_ptr<Technology>>(unitName,
					tech));

		struct requirementRest rest;
		rest.req=requirements;
		rest.vanish=vanish;
		rest.buildFrom=buildFrom;

		std::pair<std::shared_ptr<Technology>, requirementRest> unresolve(tech,rest);
		unresolvedUnitRequirements.push_front(unresolve);
	}
	if (callRest)
		initRest();
}

void TechnologyList::initBuildingList(std::string buildList)
{
	this->buildingPath=buildList;
	DataReader readFile(buildList);
	std::string line;
	bool callRest=initialized;
	while (!((line=readFile.getLine()).empty()))
	{
		initialized=true;
		std::string buildName="";
		float buildCostMinerals=0.0f;
		float buildCostGas=0.0f;
		float buildCostSupply=0.0f;
		int buildBuildTime=0;
		std::string vanish;
		std::string requirements;
		std::stringstream stream;

		stream << line;
		if (!(streamToVariable(stream, buildName)))
			continue;
		if (!(streamToVariable(stream, buildCostMinerals)))
			continue;
		if (!(streamToVariable(stream, buildCostGas)))
			continue;
		if (!(streamToVariable(stream, buildBuildTime)))
			continue;
		if (!(streamToVariable(stream, buildCostSupply)))
			continue;
		if (!(streamToVariable(stream, requirements)))
			continue;
		if (!(streamToVariable(stream, vanish)))
			continue;
		if (buildName == "Building")
			continue;

		std::shared_ptr<Technology> tech = std::make_shared<Technology>(buildName,buildCostMinerals,buildCostGas,buildCostSupply,buildBuildTime);

		buildings.insert( std::pair<std::string, std::shared_ptr<Technology>>(buildName,
					tech));

		struct requirementRest rest;
		rest.req=requirements;
		rest.vanish=vanish;

		std::pair<std::shared_ptr<Technology>, requirementRest> unresolve(tech,rest);
		unresolvedBuildingRequirements.push_front(unresolve);
	}
	if (callRest)
		initRest();
}
void TechnologyList::reset()
{
    /* no longer needed here
	for (auto it : units)
	{
		(it).second->setExistence(false);
	}
	for (auto it : buildings)
	{
		(it).second->setExistence(false);
	}
    */
	techNames.clear();
	findUnit("reset");
	findBuilding("reset");
}


void TechnologyList::initTechnologySet()
{
    if(units.size() == 0)
    {
        throw std::invalid_argument("List of units not yet initialized!");
    }
    if(buildings.size() == 0)
    {
        throw std::invalid_argument("List of buildings not yet initialized!");
    }
    for(const auto& unit : units)
    {
        techSet.insert(unit.first);
    }
    for(const auto& building : buildings)
    {
        techSet.insert(building.first);
    }
}

bool TechnologyList::isInitialized()
{
	return initialized;
}

std::vector<std::shared_ptr<Technology>> TechnologyList::findUnitVec(std::string key)
{
	std::vector<std::shared_ptr<Technology>> ret;
	auto it = units.equal_range(key).first;
	for (;it != units.equal_range(key).second;++it)
	{
		ret.push_back((*it).second);
	}
	return ret;
}
std::vector<std::shared_ptr<Technology>> TechnologyList::findBuildingVec(std::string key)
{
	std::vector<std::shared_ptr<Technology>> ret;
	auto it = buildings.equal_range(key).first;
	for (;it != buildings.equal_range(key).second;++it)
	{   
		ret.push_back((*it).second);
	}
	return ret;
}

void TechnologyList::printAll()
{
	for(auto it : units)
	{
		std::cout << (it).second << std::endl;
	}
	std::cout << std::endl;
	for(auto it : buildings)
	{
		std::cout << (it).second << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}


std::shared_ptr<Technology> TechnologyList:: findBuilding(std::string key)
{
	std::string lastKey="PlaceHolder";
	std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = buildings.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG_DEEP
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
#ifdef DEBUG_DEEP
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
	std::string lastKey="PlaceHolder";
	std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = units.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG_DEEP
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
#ifdef DEBUG_DEEP
				std::cout << "No other unit with key:\t" << key << " found\t" << count << std::endl;
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

void TechnologyList::initRandomGenerator(size_t seed, std::string SpecialOne, int weight)
{
	PROGRESS("RandomGenerator INIT");

    for(auto &it : units)
    {
        if (techNames.size()>0)
            if (it.second->getName()==techNames.back())
                continue;
        if (it.second->getName()==SpecialOne)
        {
            for (int i = 0; i < weight; ++i)
                techNames.push_back(it.second->getName());
        } else
        {
            techNames.push_back(it.second->getName());
        }
    }
    for(auto &it : buildings)
    {
        if (techNames.size()>0)
            if (it.second->getName()==techNames.back())
                continue;
        if (it.second->getName()==SpecialOne)
        {
            for (int i = 0; i < weight; ++i)
                techNames.push_back(it.second->getName());
        } else
        {
            techNames.push_back(it.second->getName());
        }
    }

    randomEngine = std::minstd_rand0(seed);
	if (!(initialized))
	{
		std::cerr << "TechList not yet initialized, cant create RandomEngine" << std::endl;
		return;
	}
	uniformDist = std::uniform_int_distribution<int>(0,techNames.size()-1);
}

std::string TechnologyList::getRandomTechnology()
{
	//unsigned int num = randomEngine()%techNames.size();
	//return techNames[num];
	return techNames[uniformDist(randomEngine)];
}

std::shared_ptr<Technology> TechnologyList::findTechnology(std::string key)
{
	std::shared_ptr<Technology> tech;
	if ((tech=findUnit(key))!=NULL)
		return tech;
	if ((tech=findBuilding(key))!=NULL)
		return tech;
	return NULL;
}

std::set<std::string> TechnologyList::getTechnologySet(void)
{
#ifdef DEBUG
    for(auto it : techSet)
    {
        std::cerr << it << std::endl;
    }
#endif
    return techSet;
}


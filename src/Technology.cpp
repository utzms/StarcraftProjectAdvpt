#include "../include/Technology.h"

Technology::Technology()
{
	setZero();
}

Technology::Technology(std::string nameIn)
{
	this->name=nameIn;
	setZero();
	PROGRESS("CONSTRUCTOR - Costs()" );
}

void Technology::setZero()
{   
	TechCosts.minerals=0.0f;
	TechCosts.gas=0.0f;
    TechCosts.supply=0;
	TechCosts.buildTime=0;
}   


Technology::Technology(std::string nameIn, float min, float gas, int supply, int buildTime)
{
#ifdef DEBUG
	std::cerr << "CONSTRUCTOR - Technology(name,float,float....)" << std::endl;
	std::cerr << "\t" << name << " " << min << " " << gas << " " << supply << " " << buildTime << std::endl;
#endif
	this->name=nameIn;
	TechCosts.minerals=min;
	TechCosts.gas=gas;
	TechCosts.supply=supply;
	TechCosts.buildTime=buildTime;
	existenceCount = 0;
}

void Technology::addRequirement(std::pair<std::shared_ptr<Technology>,RequirementType> newRequirement)
{
	std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> tmp;

	tmp.push_back(newRequirement);

	if (std::find(requirements.begin(), requirements.end(), tmp) == requirements.end())
	{
		requirements.push_back(tmp);
	}
}

void Technology::addRequirement(std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> newRequirement)
{
	if (newRequirement.size() != 0)
	{
		if (std::find(requirements.begin(), requirements.end(), newRequirement) == requirements.end())
		{
			requirements.push_back(newRequirement);
		}
	}
}
void Technology::setName(std::string nameIn)
{
	this->name=nameIn;
}   
void Technology::setMineral(float minerals)
{
	TechCosts.minerals=minerals;
}   
void Technology::setGas(float gas)
{
	TechCosts.gas=gas;
}   
void Technology::setSupply(int supply)
{
	TechCosts.supply=supply;
}   
void Technology::setBuildTime(int time)
{
	TechCosts.buildTime=time;
}   

void Technology::setExistence(int state)
{
	existenceCount = state;
}   

void Technology::incExistence()
{
	++existenceCount;
}

void Technology::decExistence()
{
	--existenceCount;
}   

std::string Technology::getName(void)
{
	return name;
}

std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> Technology::getRequirements(void)
{
	return requirements;
}
float Technology::getMineralsCost(void)
{
	return TechCosts.minerals;
}
float Technology::getGasCost(void)
{
	return TechCosts.gas;
}
int Technology::getSupplyCost(void)
{
	return TechCosts.supply;
}
int Technology::getBuildTime(void)
{
	return TechCosts.buildTime;
}

bool Technology::exists(void)
{
	return (existenceCount>0 ? true : false);
}

std::ostream& operator<< (std::ostream& os, const Technology& tech)
{
	os << std::setw(20) << tech.name;
#ifdef DEBUG
	os << std::setw(6) << tech.TechCosts.minerals;
	os << std::setw(6) << tech.TechCosts.gas;
	os << std::setw(6) << tech.TechCosts.supply;
	os << std::setw(6) << tech.TechCosts.buildTime;
	std::string tmp1="|";
	std::string tmp2="|";
	std::string tmp3="|";
	std::string *tmp;
	for (size_t it = 0; it < tech.requirements.size(); ++it)
	{
		size_t j=0;
		if (tech.requirements[it][j].second == RequirementType::Existence)
		{
			tmp = &tmp1;
		} else if (tech.requirements[it][j].second == RequirementType::ForProduction)
		{
			tmp = &tmp2;
		} else
		{
			tmp = &tmp3;
		}
		*tmp+=tech.requirements[it][j].first->getName();
		for (j=1; j < tech.requirements[it].size(); j++)
		{
			*tmp = *tmp +"/" + tech.requirements[it][j].first->getName();
		}
		*tmp = *tmp+",";
	}
	tmp1.pop_back();
	tmp2.pop_back();
	tmp3.pop_back();
	tmp1.append("|");
	tmp2.append("|");
	tmp3.append("|");
	os << std::setw(30) << tmp1;
	os << std::setw(30) << tmp2;
	os << std::setw(30) << tmp3;
#endif
	os << std::setw(15);
	if (tech.existenceCount>0)
	{
		os << "Exists(" << tech.existenceCount << ")";
	} else 
	{
		os << "No Instance";
	}

	return os;
}

std::ostream& operator<< (std::ostream& os, const Technology *tech)
{
	os << *tech;
	return os;
}

std::ostream& operator<< (std::ostream& os, const std::shared_ptr<Technology> tech)
{
	os << *tech;
#ifdef DEBUG
	os << std::setw(5) << tech.use_count();
#endif
	return os;
}


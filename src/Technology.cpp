#include <Technology.h>

Technology::Technology()
{
	setZero();
}
Technology::Technology(std::string name)
{
	name=name;
	setZero();
}
Technology::Technology(std::string name, float min, float gas, float supply, int buildTime)
{
#ifdef DEBUG
	std::cerr << "CONSTRUCTOR - Technology(name,float,float....)" << std::endl;
	std::cerr << "\t" << name << " " << min << " " << gas << " " << supply << " " << buildTime << std::endl;
#endif
	this->name=name;
	TechCosts.minerals=min;
	TechCosts.gas=gas;
	TechCosts.supply=supply;
	TechCosts.buildTime=buildTime;
    existence = false;
}

void Technology::setZero()
{
	TechCosts.minerals=0.0f;
	TechCosts.gas=0.0f;
	TechCosts.supply=0.0f;
	TechCosts.buildTime=0;
}

void Technology::addRequirement(std::shared_ptr<Technology> in)
{
	if (!(in.get()==NULL))
	{
		if (std::find(requirements.begin(), requirements.end(), in) == requirements.end())
		{
			requirements.push_back(in);
		}
	}
}
void Technology::setName(std::string name)
{
	this->name=name;
}
void Technology::setMineral(float minerals)
{
	TechCosts.minerals=minerals;
}
void Technology::setGas(float gas)
{
	TechCosts.gas=gas;
}
void Technology::setSupply(float supply)
{
	TechCosts.supply=supply;
}
void Technology::setBuildTime(int time)
{
	TechCosts.buildTime=time;
}

void Technology::setExistence(bool state) 
{
    existence = state;
}

std::string Technology::getName(void)
{
	return name;
}
std::vector<std::shared_ptr<Technology>> Technology::getRequirements(void)
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
float Technology::getSupplyCost(void)
{
	return TechCosts.supply;
}
int Technology::getBuildTime(void)
{
	return TechCosts.buildTime;
}

bool Technology::exists(void) 
{
    return existence;
}

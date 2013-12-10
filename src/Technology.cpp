#include "../include/Technology.h"

Technology::Technology()
{
}

Technology::Technology(std::string name)
    :_name(name)
{
}

Technology::Technology(std::string name, float min, float gas, float supply, int buildTime)
    :_name(name)
    ,_techCosts(Costs(min, gas, supply, buildTime))
    ,_exists(false)
{
}

void Technology::addRequirement(std::pair<std::shared_ptr<Technology>,RequirementType> newRequirement)
{
    std::vector<std::pair<std::shared_ptr <Technology>,RequirementType>> tmp;

    tmp.push_back(newRequirement);

    if (std::find(_requirements.begin(), _requirements.end(), tmp) == _requirements.end())
	{
        _requirements.push_back(tmp);
	}
}

void Technology::addRequirement(std::vector<std::pair<std::shared_ptr<Technology>,RequirementType> > newRequirement)
{
    if (newRequirement.size() != 0)
	{
        if (std::find(_requirements.begin(), _requirements.end(), newRequirement) == _requirements.end())
		{
            _requirements.push_back(newRequirement);
		}
	}
}

void Technology::setName(std::string name)
{
    _name = name;
}

void Technology::setMineral(float minerals)
{
    _techCosts.minerals = minerals;
}

void Technology::setGas(float gas)
{
    _techCosts.gas = gas;
}

void Technology::setSupply(float supply)
{
    _techCosts.supply = supply;
}

void Technology::setBuildTime(int time)
{
    _techCosts.buildTime = time;
}

void Technology::setExistence(bool state) 
{
    _exists = state;
}

std::string Technology::getName(void)
{
    return _name;
}

/*
std::vector<std::shared_ptr<Technology>> Technology::getRequirements(void)
{
	return requirements;
}
*/

std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> Technology::getRequirements(void)
{
    return _requirements;
}

float Technology::getMineralsCost(void)
{
    return _techCosts.minerals;
}

float Technology::getGasCost(void)
{
    return _techCosts.gas;
}

float Technology::getSupplyCost(void)
{
    return _techCosts.supply;
}

int Technology::getBuildTime(void)
{
    return _techCosts.buildTime;
}

bool Technology::exists(void) 
{
    return _exists;
}

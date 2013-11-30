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
	name=name;
	TechCosts.minerals=min;
	TechCosts.gas=gas;
	TechCosts.supply=supply;
	TechCosts.buildTime=buildTime;
}

inline void Technology::setZero()
{
	TechCosts.minerals=0.0f;
	TechCosts.gas=0.0f;
	TechCosts.supply=0.0f;
	TechCosts.buildTime=0;
}

inline void Technology::addRequirement(std::string in)
{
	if (!(in.empty()))
		if (std::find(requirements.begin(), requirements.end(), in) != requirements.end())
			requirements.push_back(in);
}
inline void Technology::setName(std::string name)
{
	name=name;
}
inline void Technology::setMineral(float minerals)
{
	TechCosts.minerals=minerals;
}
inline void Technology::setGas(float gas)
{
	TechCosts.gas=gas;
}
inline void Technology::setSupply(float supply)
{
	TechCosts.supply=supply;
}
inline void Technology::setBuildTime(int time)
{
	TechCosts.buildTime=time;
}

inline std::string Technology::getName(void)
{
	return name;
}
inline std::vector<std::string> Technology::getRequirements(void)
{
	return requirements;
}
inline float Technology::getMineralsCost(void)
{
	return TechCosts.minerals;
}
inline float Technology::getGasCost(void)
{
	return TechCosts.gas;
}
inline float Technology::getSupplyCost(void)
{
	return TechCosts.supply;
}
inline int Technology::getBuildTime(void)
{
	return TechCosts.buildTime;
}


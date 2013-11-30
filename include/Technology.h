#ifndef _TECHONOLGY_H_
#define _TECHONOLGY_H_


#include <vector>
#include <iostream>
#include <algorithm>

/*
 * Technology Class. Holds 'Blueprints' for each Entity (Building, Unit).
 * Basically only Set/Get of costs, requirements and name.
 *
 * TODO: Still incomplete, just demo-class - Suggestions are welcome
 *
 * */

typedef struct costs
{
	float minerals;
	float gas;
	float supply;
	int buildTime;
} costs;

class Technology
{
	private:
		std::string name;
		costs TechCosts;
		std::vector<std::string> requirements;

	public:
		Technology();
		Technology(std::string name);
		Technology(std::string name, float min, float gas, float supply, int buildTime);

		void setZero();
		void addRequirement(std::string in);
		void setName(std::string name);
		void setMineral(float minerals);
		void setGas(float gas);
		void setSupply(float supply);
		void setBuildTime(int time);

		std::string getName(void);
		std::vector<std::string> getRequirements(void);
		float getMineralsCost(void);
		float getGasCost(void);
		float getSupplyCost(void);
		int getBuildTime(void);
};

#endif

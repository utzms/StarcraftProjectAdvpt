#ifndef _TECHONOLGY_H_
#define _TECHONOLGY_H_


#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Debug.h"

/*
 * Technology Class. Holds 'Blueprints' for each Entity (Building, Unit).
 * Basically only Set/Get of costs, requirements and name.
 *
 * TODO: Still incomplete, just demo-class - Suggestions are welcome
 *
 * */

enum class RequirementType
{
	Existence,
	Vanishing,
	ForProduction
};

struct Costs
{
	float minerals;
	float gas;
	float supply;
	int   buildTime;
};

class Technology
{
	private:
		std::string name;
		Costs TechCosts;
		bool existence;

		std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> requirements;

	public:
		Technology();
		Technology(std::string name);
		Technology(std::string name, float min, float gas, float supply, int buildTime);

		void setZero();

		void addRequirement(std::pair<std::shared_ptr<Technology>, RequirementType>);
		void addRequirement(std::vector<std::pair<std::shared_ptr<Technology>, RequirementType>>);

		void setName(std::string name);
		void setMineral(float minerals);
		void setGas(float gas);
		void setSupply(float supply);
		void setBuildTime(int time);
		void setExistence(bool state);
		std::string getName(void);


		std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> getRequirements(void);
		float getMineralsCost(void);
		float getGasCost(void);
		//getSupplyCost returns providing supply in case of buildings
		float getSupplyCost(void);
		int getBuildTime(void);
		bool exists();

};

#endif

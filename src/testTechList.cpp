#include <iomanip> 
#include <TechnologyList.h>
#include "InitTechTree.hpp"
#include "Technology.h"

int test()
{
	TechnologyList tech;
	InitTechTree<ProtossTechTree> initTech(&tech);
	initTech.initTechTree();

	Technology *tec = tech.findUnit("Probe").get();
	tec = tech.findUnit("Probe").get();

	std::vector<std::string> names;
	names.push_back("Probe");
	names.push_back("Zealot");
	names.push_back("Observer");
	names.push_back("HighTemplar");
	names.push_back("Mothership");
	names.push_back("VoidRay");
	names.push_back("Nexus");
	names.push_back("PhotonCannon ");
	names.push_back("TemplarArchives");
	names.push_back("RoboticsBay");
	names.push_back("Assimilator");
	names.push_back("Warpgate");
	names.push_back("");
	names.push_back("");
	names.push_back("");
	names.push_back("");
	names.push_back("");
	for (size_t run = 0; run < names.size(); ++run)
	{
		std::vector<std::shared_ptr<Technology>> r = tech.findUnitVec(names[run]);
		if (r.size()==0)
		{
			r=tech.findBuildingVec(names[run]);
		}
		for (size_t i = 0; i < r.size(); ++i)
		{
			std::cout << std::setw(15);
			std::cout << r[i]->getName() << "\t";
			std::cout << r[i]->getMineralsCost() << "\t";
			std::cout << r[i]->getGasCost() << "\t";
			std::cout << r[i]->getSupplyCost() << "\t";
			std::cout << r[i]->getBuildTime() << "\t";
			std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> req = r[i]->getRequirements();
			for(size_t ii = 0; ii < req.size(); ++ii)
			{
				std::cout << std::setw(20);
				size_t j=0;
				{
					std::string s="";
					if (!(ii >= req.size()))
						while (req[ii][j].second == RequirementType::Existence)
						{
							s += req[ii][j].first->getName()+"/";
							++j;
						}
					if (j!=0)
						ii++;
					j=0;
					std::cout << "-" << s << "-\t";
					s="";
					if (!(ii >= req.size()))
						while (req[ii][j].second == RequirementType::ForProduction)
						{
							s += req[ii][j].first->getName()+"/";
							++j;
						}
					if (j!=0)
						ii++;
					j=0;
					std::cout << "?"<< s << "?\t";
					s="";
					if (!(ii >= req.size()))
						while (req[ii][j].second == RequirementType::Vanishing)
						{
							s += req[ii][j].first->getName()+"/";
							++j;
						}
					std::cout << "!"<< s << "!\t";
				}
			}

			std::cout << std::endl;
		}
	}
	return 0;
}


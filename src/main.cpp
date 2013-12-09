#include <Simulation.h>
#include <iomanip> //include this for formatted output
#include <TechnologyList.h>
#include "InitTechTree.hpp"
#include "Technology.h"

int main()
{
	//Simulation simulation;
	TechnologyList tech;
	InitTechTree<ProtossTechTree> initTech(&tech);
	initTech.initTechTree();

	Technology *tec = tech.findUnit("Probe").get();
	tec = tech.findUnit("Probe").get();
	/*
	   while (((tec = tech.findUnit("Archon").get()) != NULL))
	   {
	   std::cout << tec->getName() << "\t";
	   std::cout << tec->getMineralsCost() << "\t";
	   std::cout << tec->getGasCost() << "\t";
	   std::cout << tec->getSupplyCost() << "\t";
	   std::cout << tec->getBuildTime() << "\t";
	   std::vector<std::vector<std::shared_ptr<Technology>>> req = tec->getRequirements();
	   for(size_t i = 0; i < req.size(); ++i)
	   {
	   for (size_t j = 0; j < req[i].size(); ++j)
	   std::cout << req[i][j]->getName() <<"/"; 
	   std::cout << ",";
	   }
	   std::cout << std::endl;
	   }
	   while (((tec=tech.findBuilding("CyberneticsCore").get()) != NULL))
	   {
	   std::cout << tec->getName() << "\t";
	   std::cout << tec->getMineralsCost() << "\t";
	   std::cout << tec->getGasCost() << "\t";
	   std::cout << tec->getSupplyCost() << "\t";
	   std::cout << tec->getBuildTime() << "\t";
	   std::vector<std::vector<std::shared_ptr<Technology>>> req = tec->getRequirements();
	   for(size_t i = 0; i < req.size(); ++i)
	   {
	   for (size_t j = 0; j < req[i].size(); ++j)
	   std::cout << req[i][j]->getName() <<"/";
	   std::cout << ",";
	   }
	   std::cout << std::endl;
	   }
	 */
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
			std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementFlag>>> req = r[i]->getRequirements();
			for(size_t ii = 0; ii < req.size(); ++ii)
			{
				std::cout << std::setw(20);
				size_t j=0;
				//for (size_t j = 0; j < req[ii].size(); ++j)
				{
					std::string s="";
					if (!(ii >= req.size()))
					while (req[ii][j].second == Existent)
					{
						//std::cout << req[i][j].first->getName() << "/";
						s += req[ii][j].first->getName()+"/";
						++j;
					}
					if (j!=0)
					ii++;
					j=0;
					std::cout << "-" << s << "-\t";
					s="";
					if (!(ii >= req.size()))
					while (req[ii][j].second == Creation)
					{
						//std::cout << req[i][j].first->getName() << "|";
						s += req[ii][j].first->getName()+"/";
						++j;
					}
					if (j!=0)
						ii++;
					j=0;
					std::cout << "?"<< s << "?\t";
					s="";
					if (!(ii >= req.size()))
					while (req[ii][j].second == Vanish)
					{
						//std::cout << req[i][j].first->getName() << "-";
						s += req[ii][j].first->getName()+"/";
						++j;
					}
					std::cout << "!"<< s << "!\t";
				}
			}

			std::cout << std::endl;
		}
	}
}

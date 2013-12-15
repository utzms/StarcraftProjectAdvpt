#ifndef _INITTECHTREE_HPP_
#define _INITTECHTREE_HPP_

#include "RacePolicy.h"
#include "TechnologyList.h"

/*
   Interface to Technology Trees, working with Policies defined in techTreePolicy.hh
   This class builds the TechTree and all dependencies if needed files are present. (Paths are hardcoded in Policy Helperfile)
   */

template<typename TechTree>
class InitTechTree : private TechTree
{
	private:
        std::shared_ptr<TechnologyList> tech;
	public:
		InitTechTree(){}
		InitTechTree(std::shared_ptr<TechnologyList> l){this->tech=l;}

		void setTechnologyList(std::shared_ptr<TechnologyList> l)
		{
			this->tech=l;
		}
		bool initTechTree() const
		{
			if (!(tech==NULL))
			{
				tech->initBuildingList(TechTree::buildingPath1());
				if (!(tech->isInitialized()))
				{
					std::cerr << "Will try another Path" << std::endl;
					tech->initBuildingList(TechTree::buildingPath2());
					tech->initUnitList(TechTree::unitPath2());
				} else
				{
					tech->initUnitList(TechTree::unitPath1());
				}
				if (!(tech->isInitialized()))
					return false;
				return true;
			} else {
				std::cerr << "No valid Tech List used" << std::endl;
				return false;
			}
		}

};

#endif

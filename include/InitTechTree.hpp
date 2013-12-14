#ifndef _INITTECHTREE_HPP_
#define _INITTECHTREE_HPP_

#include "techTreePolicy.h"
#include "TechnologyList.h"

/*
   Interface to Technology Trees, working with Policies defined in techTreePolicy.hh
   This class builds the TechTree and all dependencies if needed files are present. (Paths are hardcoded in Policy Helperfile)
   */

template<typename TechTree>
class InitTechTree : private TechTree
{
	private:
		TechnologyList *tech;
	public:
		InitTechTree(){}
		InitTechTree(TechnologyList *l){this->tech=l;}

		void setTechnologyList(TechnologyList *l)
		{
                bool result = false;

                _technologyList->initBuildingList(_technologyList->getBuildingPath());
                _technologyList->initUnitList(_technologyList->getUnitPath());


                if (_technologyList->isInitialized())
                {
                    result = true;
                }
				else
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

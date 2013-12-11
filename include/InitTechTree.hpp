#ifndef _INITTECHTREE_HPP_
#define _INITTECHTREE_HPP_

#include "TechnologyList.h"

/*
   Interface to Technology Trees, working with Policies defined in techTreePolicy.hh
   This class builds the TechTree and all dependencies if needed files are present. (Paths are hardcoded in Policy Helperfile)
   */

class InitTechTree
{
	private:
        std::shared_ptr<TechnologyList> _technologyList;

	public:
        InitTechTree()
        {
        }

        InitTechTree(std::shared_ptr<TechnologyList> technologyList)
            :_technologyList(technologyList)
        {
        }

        bool initTechTree()
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
					std::cerr << "No valid Tech List used" << std::endl;
				  result = false;
				}

            return result;
		}

};

#endif

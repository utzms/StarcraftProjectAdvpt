#ifndef _INITTECHTREE_HPP_
#define _INITTECHTREE_HPP_

#include "techTreePolicy.h"
#include "TechnologyList.h"

/*
   Interface to Technology Trees, working with Policies defined in techTreePolicy.hh
   This class builds the TechTree and all dependencies if needed files are present. (Paths are hardcoded in Policy Helperfile)
   */

template<typename TechTree>
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

            if (_technologyList)
			{
                _technologyList->initBuildingList(TechTree::buildingPath1());

                if (!(_technologyList->isInitialized()))
				{
					std::cerr << "Will try another Path" << std::endl;
                    _technologyList->initBuildingList(TechTree::buildingPath2());
                    _technologyList->initUnitList(TechTree::unitPath2());
                }
                else
				{
                    _technologyList->initUnitList(TechTree::unitPath1());
				}

                if (_technologyList->isInitialized())
                {
                    result = true;
                }
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

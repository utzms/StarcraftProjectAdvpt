#ifndef _INITTECHTREE_HPP_
#define _INITTECHTREE_HPP_

#include "techTreePolicy.hh"
#include "TechnologyList.h"

template<typename TechTree>
class InitTechTree : private TechTree
{
	using TechTree::unitPath1;
	private:
		TechnologyList *tech;
	public:
		InitTechTree(){}
		InitTechTree(TechnologyList *l){this->tech=l;}

		void initUnitList() const
		{
			if (!(tech==NULL))
			{
				tech->initUnitList(TechTree::unitPath1());
				if (!(tech->isInitialized()))
				{
					std::cerr << "Couldnt open file" << std::endl;
					std::cerr << "Will try another Path" << std::endl;
					tech->initUnitList(TechTree::unitPath2());
				}
			} else {
				std::cerr << "No valid Tech List used" << std::endl;
			}
		}

};

#endif

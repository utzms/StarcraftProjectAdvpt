
#ifndef _TECHTREEPOLICY_HH_
#define _TECHTREEPOLICY_HH_

#include <iostream>
#include <string>

class ProtossTechTree
{
	protected:
		std::string unitPath1() const
		{
			return "../data/Protoss/units.txt";
		}
		std::string unitPath2() const
		{
			return "./data/Protoss/units.txt";
		}
};

class ZergTechTree
{
	protected:
		std::string unitPath1() const
		{
			return "./data/Zerg/units.txt";
		}
};

class TerranTechTree
{
	protected:
		std::string unitPath1() const
		{
			return "./data/Terran/units.txt";
		}
};

#endif


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
		std::string buildingPath1() const
		{
			return "../data/Protoss/buildings.txt";
		}
		std::string buildingPath2() const
		{
			return "./data/Protoss/buildings.txt";
		}
};

class ZergTechTree
{
	protected:
		std::string unitPath1() const
		{
			return "../data/Zerg/units.txt";
		}
		std::string unitPath2() const
		{
			return "./data/Zerg/units.txt";
		}
		std::string buildingPath1() const
		{
			return "../data/Zerg/buildings.txt";
		}
		std::string buildingPath2() const
		{
			return "./data/Zerg/buildings.txt";
		}
};

class TerranTechTree
{
	protected:
		std::string unitPath1() const
		{
			return "../data/Terran/units.txt";
		}
		std::string unitPath2() const
		{
			return "./data/Terran/units.txt";
		}
		std::string buildingPath1() const
		{
			return "../data/Terran/buildings.txt";
		}
		std::string buildingPath2() const
		{
			return "./data/Terran/buildings.txt";
		}
};

#endif

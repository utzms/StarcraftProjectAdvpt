
#ifndef _TECHTREEPOLICY_H_
#define _TECHTREEPOLICY_H_

#include <iostream>
#include <string>

class ProtossTechTree
{
    public:
        static std::string unitPath1()
		{
			return "../data/Protoss/units.txt";
		}

        static std::string unitPath2()
		{
			return "./data/Protoss/units.txt";
		}

        static std::string buildingPath1()
		{
			return "../data/Protoss/buildings.txt";
		}

        static std::string buildingPath2()
		{
			return "./data/Protoss/buildings.txt";
		}
};

class ZergTechTree
{
    public:
        static std::string unitPath1()
		{
			return "../data/Zerg/units.txt";
		}

        static std::string unitPath2()
		{
			return "./data/Zerg/units.txt";
		}

        static std::string buildingPath1()
		{
			return "../data/Zerg/buildings.txt";
		}

        static std::string buildingPath2()
		{
			return "./data/Zerg/buildings.txt";
		}
};

class TerranTechTree
{
    public:
        static std::string unitPath1()
		{
			return "../data/Terran/units.txt";
		}

        static std::string unitPath2()
		{
			return "./data/Terran/units.txt";
		}

        static std::string buildingPath1()
		{
			return "../data/Terran/buildings.txt";
		}

        static std::string buildingPath2()
		{
			return "./data/Terran/buildings.txt";
		}
};

#endif

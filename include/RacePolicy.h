
#ifndef _RACEPOLICY_H_
#define _RACEPOLICY_H_

#include <iostream>
#include <string>


enum class RaceType
{
	Protoss,
	Terran,
	Zerg
};

class Protoss
{
    public:
		static RaceType getRace()
		{
			return RaceType::Protoss;
		}

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
		static std::string getMainBuilding()
		{
			return "Nexus";
		}

//		static void upgradeMainBuilding(std::string newName)
//		{
//		}

		static std::string getWorker()
		{
			return "Probe";
		}
		static std::string getGasHarvestBuilding()
		{
			return "Assimilator";
		}
		static std::string getSupplyProvider()
		{
			return "Pylon";
		}
};

class Zerg
{
    public:
		static RaceType getRace()
		{
			return RaceType::Zerg;
		}

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
		static std::string getMainBuilding()
		{
			return "Hatchery";
		}



		static std::string getWorker()
		{
			return "Drone";
		}
		static std::string getGasHarvestBuilding()
		{
			return "Extractor";
		}
		static std::string getSupplyProvider()
		{
			return "Overlord";
		}
		
};

class Terran
{
    public:
		static RaceType getRace()
		{
			return RaceType::Terran;
		}
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
		static std::string getMainBuilding()
		{
			return "CommandCenter";
		}
		static std::string getWorker()
		{
			return "SCV";
		}
		static std::string getGasHarvestBuilding()
		{
			return "Refinery";
		}
		static std::string getSupplyProvider()
		{
			return "SupplyDepot";
		}
};

#endif

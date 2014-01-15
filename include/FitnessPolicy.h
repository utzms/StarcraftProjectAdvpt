
#ifndef _FITNESSPOLICY_H_
#define _FITNESSPOLICY_H_

#include <iostream>
#include <string>

#include <Debug.h>
#include <Simulation.h>

enum class FitnessType
{
	Push,
	Rush
};

//how many targets can be built in a given time
class Push
{
	public:
		std::string m_Target;
		int m_Time;

		static FitnessType getFitnessType()
		{
			return FitnessType::Push;
		}

		//returns the rating of the buildlist counting hard constraints
		static int rateBuildListHard(const SimulationResult &simulationResult)
		{
			int rating = 0;
			//count targets in the list
			for(auto &i : simulationResult->resultList)
			{
				//TODO if simulation doesn't quit after the time constraint, this should be checked here
				if(m_Target.compare(simulationResult->resultList.second) == 0)
				{
					PROGRESS("Found target in the list");
					++rating;
				}
			}

			return rating;
		}

		//returns the rating of the buildlist counting soft contraints
		static int rateBuildListSoft(const SimulationResult &simulationResult)
		{
			//TODO
			int rating = 0;
			

			return rating;
		}

};

//build as fast as possible a special amount of the target
class Rush
{
	public:
		std::string m_Target;
		int m_Number;

		static FitnessType getFitnessType()
		{
			return FitnessType::Rush;
		}

		//returns the rating of the buildlist counting hard constraints
		static int rateBuildListHard(const SimulationResult &simulationResult)
		{
			//TODO
			
			return 0;
		}

		//returns the rating of the buildlist counting soft constraints
		static int rateBuildListSoft(const SimulationResult &simulationResult)
		{
			//TODO


			return 0;
		}

};

#endif

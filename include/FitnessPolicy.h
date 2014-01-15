
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
		int m_Time; //its the time limit
		int m_Number;

		Push(std::string target, int time, int number)
		{
			m_Target = target;
			m_Time = time;
			m_Number = number;
		}

		FitnessType getFitnessType()
		{
			return FitnessType::Push;
		}

		//returns the rating of the buildlist counting hard constraints
		int rateBuildListHard(const SimulationResult &simulationResult)
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
				//TODO maybe the rest of the list should be deleted
			}

			return rating;
		}

		//returns the rating of the buildlist counting soft contraints
		int rateBuildListSoft(const SimulationResult &simulationResult)
		{
			//TODO minerals, vespene, number of workers,...
			int rating = 0;
			

			return rating;
		}

};

//build as fast as possible a special amount of the target
class Rush
{
	public:
		std::string m_Target;
		int m_Time;
		int m_Number; //its the number of targets

		Rush(std::string target, int time, int number)
		{
			m_Target = target;
			m_Time = time;
			m_Number = number;
		}

		FitnessType getFitnessType()
		{
			return FitnessType::Rush;
		}

		//returns the rating of the buildlist counting hard constraints
		int rateBuildListHard(const SimulationResult &simulationResult)
		{
			int rating = 0;
			int count = 0;
			for(auto &i : simulationResult->resultList)
			{
				//TODO if simulation doesn't quit after the number constraint, this should be checked here
				//TODO the time aspekt should be added to the rating
				if(m_Target.compare(simulationResult->resultList.second) == 0)
				{
					PROGRESS("Found target in the list");
					++count;
					rating += 10; //testwert
				}
				if(count >= m_SecondConstraint)
				{
					rating += 30; //testwert
				}
				else //not very nice...
				{
					rating -= 5; //testwert
				}
				//TODO maybe the rest of the list should be deleted 
			}


			return rating;
		}

		//returns the rating of the buildlist counting soft constraints
		int rateBuildListSoft(const SimulationResult &simulationResult)
		{
			//TODO minerals, vespene, number of workers,...
			int rating = 0;


			return rating;
		}

};

#endif

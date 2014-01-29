
#ifndef _FITNESSPOLICY_H_
#define _FITNESSPOLICY_H_

#include <iostream>
#include <string>

#include "Debug.h"
#include "Simulation.h"

enum class FitnessType
{
	Push,
	Debug,
	Rush
};


class Debug
{
	public:
		std::string m_Target;
		int m_Time; //its the time limit
		int m_Number;

		Debug(std::string target, int time, int number)
		{
			m_Target = target;
			m_Time = time;
			m_Number = number;
		}

		FitnessType getFitnessType()
		{
			return FitnessType::Debug;
		}

		//returns the rating of the buildlist counting hard constraints
        int rateBuildListHard(std::map<int, std::string> &resultList)
		{
			auto a = resultList;
			static int rating = 0;

			return ++rating;
		}

		//returns the rating of the buildlist counting soft contraints
        int rateBuildListSoft(std::map<int, std::string> &resultList, std::string worker, std::vector<std::string> requirements)
		{
			//number of workers, number of production buildings...
			static int rating = 50000000;
			auto a = resultList;
			auto b = worker;
			auto c = requirements;
			return rating--;
		}
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
        int rateBuildListHard(std::map<int, std::string> &resultList)
		{
			int rating = 0;
			//count targets in the list
			for(auto i : resultList)
			{
				//TODO if simulation doesn't quit after the time constraint, this should be checked here
				if(m_Target.compare(i.second) == 0)
				{
					PROGRESS("Found target in the list");
					++rating;
				}
				//TODO maybe the rest of the list should be deleted
			}

			return rating;
		}

		//returns the rating of the buildlist counting soft contraints
        int rateBuildListSoft(std::map<int, std::string> &resultList, std::string worker, std::vector<std::string> requirements)
		{
			//number of workers, number of production buildings...
			int rating = 0;
			for(auto i : resultList)
			{
				if(worker.compare(i.second) == 0)
				{
					++rating;
				}
				for(auto j : requirements)
				{
					if(j.compare(i.second))
					{
						++rating;
					}
				}
			}

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
		int m_SecondConstraint;

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
        int rateBuildListHard(std::map<int, std::string> &resultList)
		{
			int rating = 0;
			int count = 0;
			m_SecondConstraint=400;
			for(auto i : resultList)
			{
				//TODO if simulation doesn't quit after the number constraint, this should be checked here
				//TODO the time aspekt should be added to the rating
				if(m_Target.compare(i.second) == 0)
				{
					PROGRESS("Found target in the list");
					++count;
					rating += 10; //testwert
				}
				if(count >= m_SecondConstraint)
				{
					rating += 50; //testwert
				}
				//TODO maybe the rest of the list should be deleted 
			}


			return rating;
		}

		//returns the rating of the buildlist counting soft constraints
        int rateBuildListSoft(std::map<int, std::string> &resultList, std::string worker, std::vector<std::string> requirements)
		{
			//number of workers, number of production buildings...
			int rating = 0;
			for(auto i : resultList)
			{
				if(worker.compare(i.second) == 0)
				{
					++rating;
				}
				for(auto j : requirements)
				{
					if(j.compare(i.second))
					{
						++rating;
					}
				}
			}
            return rating;
		}
};

#endif

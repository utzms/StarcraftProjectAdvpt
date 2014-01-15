
#ifndef _FITNESSPOLICY_H_
#define _FITNESSPOLICY_H_

#include <iostream>
#include <string>


enum class FitnessType
{
	Push,
	Rush
};

class Push
{
	public:
		std::string m_Target;
		int m_Time;

		static FitnessType getFitnessType()
		{
			return FitnessType::Push;
		}

};

class Rush
{
	public:
		std::string m_Target;
		int m_Number;

		static FitnessType getFitnessType()
		{
			return FitnessType::Rush;
		}

};

#endif

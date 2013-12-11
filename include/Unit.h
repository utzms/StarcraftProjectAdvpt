#ifndef _UNIT_H_
#define _UNIT_H_
#include "Entity.h"

class Unit
{   
    private:
		std::string _name;

	public:
		std::string getName()
		{
			return _name;
		}
};
#endif

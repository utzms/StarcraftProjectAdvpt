#ifndef _UNIT_H_
#define _UNIT_H_

class Unit
{   
    private:
		std::string _name;

	public:
		Unit(std::string name)
		{
			_name = name;
		}

		std::string getName()
		{
			return _name;
		}
};

#endif // _UNIT_H_

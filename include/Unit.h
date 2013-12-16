#ifndef _UNIT_H_
#define _UNIT_H_

class Unit
{   
    private:
		std::string _name;

	public:

		enum class State{
			Ready,Morphing
		};

		State state;
		std::string morphTargetName;
		int timer;

		Unit(std::string name):_name(name),
								state(State::Ready),
								timer(0)
		{
		}

		std::string getName()
		{
			return _name;
		}

		void timeStep()
		{
			if (state == State::Morphing)
			{
				timer--;
			}
		}
};

#endif // _UNIT_H_

#ifndef _BUILDING_H_
#define _BUILDING_H_

class Building
{
	public:
		enum class ProductionType
		{
			WorkerOrder,
			UnitOrder
		};

	private:
		std::string _name;


	public:
        enum class State
        {
            UnderConstruction,
            Ready,
            Producing
        };

		State state;
		int			timer;
		std::string productionUnitName;
		ProductionType productionType;


		Building(std::string name, int buildTime)
			:_name(name)
			,timer(buildTime)
			,state(State::UnderConstruction)
		{
		}

        void timeStep()
        {
			if (state != State::Ready)
			{
				timer--;
			}
        }

        int getTime()
        {
			return timer;
        }

		std::string getName()
		{
			return _name;
		}
};

#endif

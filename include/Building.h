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
		int			_timer;
		std::string _productionUnitName;

		ProductionType _productionType;

	public:
        enum class State
        {
            UnderConstruction,
            Ready,
            Producing
        };

		State state;

		Building(std::string name, int buildTime)
			:_name(name)
			,_timer(buildTime)
			,state(State::UnderConstruction)
		{
		}

        void timeStep()
        {
			if (state != State::Ready)
			{
				_timer--;
			}
        }

        int getTime()
        {
			return _timer;
        }

		std::string getName()
		{
			return _name;
		}
};

#endif

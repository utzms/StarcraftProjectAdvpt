#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <queue>

class Building
{
	private:	
        template <class EntityType>
        struct Order
        {
            int timer;

            EntityType* finish()
            {
                return new EntityType;
            }

            Order()
                :timer(0)
            {

            }
        };

        std::queue<Order> orderQueue;

		int timer;
        int maxOrders;

	public:
        enum class State
        {
            UnderConstruction,
            Ready,
            Producing
        };

        template <class EntityType>
        void order()
        {
        }

        void timeStep()
        {

        }

        int getTime()
        {
            return timer;
        }
};

#endif

#ifndef _ENTITY_H_
#define _ENTITY_H_
#include <string>
/** Abstract class all specific types of entities are derived from */


// we don't need this abstract base class, we can do all of this
// through template argument deduction in technologymanager
// and we have separate lists of the workers, buildings and units
// in the GameState anyhow

//TODO: This class is not virtual!!

class Entity
{   
    private:
        std::string name;

    public:
        std::string getName();
};

#endif

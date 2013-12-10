include_directories( . )

set ( OWN_SOURCES  Building.cpp DataReader.cpp Debug.cpp main.cpp GameState.cpp ResourceManager.cpp Simulation.cpp Technology.cpp TechnologyList.cpp TechnologyManager.cpp Unit.cpp )
set ( OWN_HEADERS 
  ../include/Building.h
  ../include/BuildList.h
  ../include/DataReader.h
  ../include/Debug.h
  ../include/GameState.h
  ../include/InitTechTree.hpp
  ../include/ResourceManager.h
  ../include/Simulation.h
  ../include/Technology.h
  ../include/TechnologyList.h
  ../include/TechnologyManager.h
  ../include/techTreePolicy.h
  ../include/Unit.h
  ../include/Worker.h
  ../include/Entity.h )
  
  
  add_executable( adpvt ${OWN_SOURCES} ${OWN_HEADERS} )
 

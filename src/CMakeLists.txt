include_directories( . )

set ( OWN_SOURCES  
	DataReader.cpp 
	Debug.cpp 
	main.cpp 
	ResourceManager.cpp 
	Technology.cpp 
	Simulation.cpp
	TechnologyList.cpp )

set ( OWN_HEADERS 
  ../include/Building.h
  ../include/BuildList.h
  ../include/DataReader.h
  ../include/Debug.h
  ../include/GameState.h
  ../include/GameStateUpdate.h
  ../include/InitTechTree.hpp
  ../include/ResourceManager.h
  ../include/Simulation.h
  ../include/StartingConfiguration.h
  ../include/Technology.h
  ../include/TechnologyList.h
  ../include/TechnologyManager.h
  ../include/Unit.h
  ../include/Worker.h
  ../include/Entity.h )
  
  
  add_executable( adpvt ${OWN_SOURCES} ${OWN_HEADERS} )
 

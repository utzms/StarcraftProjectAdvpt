
.PHONY: pathmaker clean

CC=g++
OPT=-std=c++11 -Wall -Wshadow -pedantic
LIBS=
INCPATH=./include/
SRCPATH=./src/
INC=-I$(INCPATH)
BINPATH=./bin/
OBJPATH=./obj/

objects=$(SRCPATH)main.cpp\
		$(OBJPATH)debug.o\
		$(OBJPATH)dataReader.o\
		$(OBJPATH)technology.o\
		$(OBJPATH)technologylist.o\
		$(INCPATH)InitTechTree.hpp

out=$(OBJPATH)worker.o\
		$(OBJPATH)building.o\
		$(OBJPATH)technologyManager.o\

all: exe

test: OPT += -DTECHTREETEST
test: OPT += -DDEBUG
test: exe

debug: OPT += -DDEBUG
debug: exe

pathmaker:
	mkdir -p obj/ bin/

exe: pathmaker $(objects)
	$(CC) $(OPT) $(INC) $(LIBS) -o $(BINPATH)runme $(objects)

#exe: pathmaker $(SRCPATH)main.cpp $(OBJPATH)simulation.o $(OBJPATH)gameState.o $(OBJPATH)worker.o $(OBJPATH)building.o $(OBJPATH)technologyManager.o $(OBJPATH)dataReader.o $(OBJPATH)technology.o $(OBJPATH)technologylist.o $(INCPATH)InitTechTree.hpp
#	$(CC) $(OPT) $(INC) $(LIBS) -o $(BINPATH)runme $(SRCPATH)main.cpp $(OBJPATH)simulation.o $(OBJPATH)gameState.o $(OBJPATH)worker.o $(OBJPATH)building.o $(OBJPATH)technologyManager.o $(OBJPATH)dataReader.o $(OBJPATH)technology.o $(OBJPATH)technologylist.o $(INCPATH)InitTechTree.hpp

$(OBJPATH)debug.o: $(INCPATH)Debug.h $(SRCPATH)Debug.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)debug.o $(SRCPATH)Debug.cpp

$(OBJPATH)simulation.o: $(INCPATH)Simulation.h $(SRCPATH)Simulation.cpp $(INCPATH)GameState.h $(INCPATH)ResourceManager.h $(INCPATH)TechnologyManager.h $(INCPATH)Entity.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)simulation.o $(SRCPATH)Simulation.cpp

$(OBJPATH)worker.o: $(INCPATH)Worker.h $(INCPATH)Entity.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)worker.o $(SRCPATH)Worker.cpp

$(OBJPATH)building.o: $(INCPATH)Building.h $(INCPATH)Entity.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)building.o $(SRCPATH)Building.cpp

$(OBJPATH)gameState.o: $(INCPATH)Building.h $(INCPATH)Entity.h $(INCPATH)Unit.h $(SRCPATH)GameState.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)gameState.o $(SRCPATH)GameState.cpp

$(OBJPATH)technologyManager.o: $(INCPATH)TechnologyManager.h $(SRCPATH)TechnologyManager.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)technologyManager.o $(SRCPATH)TechnologyManager.cpp

$(OBJPATH)dataReader.o: $(INCPATH)DataReader.h $(SRCPATH)DataReader.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)dataReader.o $(SRCPATH)DataReader.cpp

$(OBJPATH)technology.o: $(INCPATH)Technology.h $(SRCPATH)Technology.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)technology.o $(SRCPATH)Technology.cpp

$(OBJPATH)technologylist.o: $(INCPATH)TechnologyList.h $(SRCPATH)TechnologyList.cpp $(INCPATH)Technology.h $(INCPATH)DataReader.h $(INCPATH)techTreePolicy.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)technologylist.o $(SRCPATH)TechnologyList.cpp


clean:
	rm -rf $(OBJPATH) $(BINPATH)

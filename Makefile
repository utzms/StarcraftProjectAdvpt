
.PHONY: pathmaker clean

CC=g++
OPT= -g -O2 -std=c++11 #-Wall -Wshadow -Wextra -pedantic
LIBS=-fopenmp
INCPATH=./include/
SRCPATH=./src/
INC=-I$(INCPATH) -I$(SRCPATH)
BINPATH=./bin/
OBJPATH=./obj/

#OPT += -O2

objects=$(SRCPATH)main.cpp\
		$(OBJPATH)debug.o\
		$(OBJPATH)dataReader.o\
		$(OBJPATH)resourcemanager.o\
		$(OBJPATH)technology.o\
		$(OBJPATH)simulation.o\
		$(OBJPATH)technologylist.o\
		$(OBJPATH)buildlistgenerator.o\
		$(INCPATH)FitnessPolicy.h\
		$(INCPATH)InitTechTree.hpp

out=$(OBJPATH)worker.o\
		$(OBJPATH)building.o\
		$(OBJPATH)technologyManager.o\
		$(OBJPATH)buildlistoptimizer.o\

all: OPT += -DNDEBUG
all: exe

debug_deep: OPT += -DDEBUG_DEEP
debug_deep: debug

debug: OPT += -DDEBUG
debug: exe

pathmaker:
	mkdir -p obj/ bin/

exe: pathmaker $(objects)
	$(CC) $(OPT) $(INC) $(LIBS) -o $(BINPATH)runme $(objects)

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

$(OBJPATH)technologylist.o: $(INCPATH)TechnologyList.h $(SRCPATH)TechnologyList.cpp $(INCPATH)Technology.h $(INCPATH)DataReader.h $(INCPATH)RacePolicy.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)technologylist.o $(SRCPATH)TechnologyList.cpp

$(OBJPATH)resourcemanager.o: $(SRCPATH)ResourceManager.cpp $(INCPATH)ResourceManager.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)resourcemanager.o $(SRCPATH)ResourceManager.cpp

$(OBJPATH)buildlistgenerator.o: $(INCPATH)BuildListGenerator.h $(SRCPATH)BuildListGenerator.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)buildlistgenerator.o $(SRCPATH)BuildListGenerator.cpp

$(OBJPATH)buildlistoptimizer.o: $(INCPATH)BuildListOptimizer.h $(SRCPATH)BuildListOptimizer.cpp
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)buildlistoptimizer.o $(SRCPATH)BuildListOptimizer.cpp

clean:
	rm -rf $(OBJPATH) $(BINPATH)

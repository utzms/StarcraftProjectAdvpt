
CC=g++
OPT=-Wall -pedantic
LIBS=
INCPATH=./include/
SRCPATH=./src/
INC=-I$(INCPATH)
BINPATH=./bin/
OBJPATH=./obj/

all: exe

debug: OPT += -DDEBUG
debug: exe

exe: $(SRCPATH)main.cpp $(OBJPATH)simulation.o $(OBJPATH)gameState.o $(OBJPATH)worker.o $(OBJPATH)building.o $(OBJPATH)technologyManager.o $(OBJPATH)dataReader.o $(OBJPATH)technology.o $(OBJPATH)technologylist.o
	$(CC) $(OPT) $(INC) $(LIBS) -o $(BINPATH)runme $(SRCPATH)main.cpp $(OBJPATH)simulation.o $(OBJPATH)gameState.o $(OBJPATH)worker.o $(OBJPATH)building.o $(OBJPATH)technologyManager.o $(OBJPATH)dataReader.o $(OBJPATH)technology.o $(OBJPATH)technologylist.o

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

$(OBJPATH)technologylist.o: $(INCPATH)TechnologyList.h $(SRCPATH)TechnologyList.cpp $(INCPATH)Technology.h $(INCPATH)DataReader.h
	$(CC) $(OPT) $(INC) $(LIBS) -c -o $(OBJPATH)technologylist.o $(SRCPATH)TechnologyList.cpp

clean:
	rm -f $(OBJPATH)* $(BINPATH)*
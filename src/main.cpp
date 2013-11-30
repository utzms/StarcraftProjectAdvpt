#include <Simulation.h>
#include <DataReader.h>

int main()
{
	Simulation simulation;
	DataReader dat("./data/Protoss/units.txt");
	DataReader d;
	dat.getLine();
	d.getLine();
}

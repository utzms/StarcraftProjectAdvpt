#ifndef STARTINGCONFIGURATION_H
#define STARTINGCONFIGURATION_H

class StartingConfiguration
{
	private:

		int _initialWorkerCount;
		int _initialMinerals;
		int _initialVespeneGas;

	public:

		StartingConfiguration(std::string filename)
		{
			std::fstream configFile(filename, std::ios::in);

			if(!configFile.good())
			{
				throw std::invalid_argument("file for StartingConfiguration is not valid");
			}

			std::string currentString;
			while(configFile >> currentString)
			{
				if(currentString.compare("Workers") == 0)
				{
					configFile >> _initialWorkerCount;
				}
				else if(currentString.compare("Minerals") == 0)
				{
					configFile >> _initialMinerals;
				}
				else if(currentString.compare("VespeneGas") == 0)
				{
					configFile >> _initialVespeneGas;
				}
			}

		}

		inline int getInitialWorkerCount()
		{
			return _initialWorkerCount;
		}

		inline int getInitialMinerals()
		{
			return _initialWorkerCount;
		}

		inline int getInitialVespeneGas()
		{
			return _initialVespeneGas;
		}

};
#endif // STARTINGCONFIGURATION_H

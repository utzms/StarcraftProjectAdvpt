#ifndef STARTINGCONFIGURATION_H
#define STARTINGCONFIGURATION_H

class StartingConfiguration
{
private:

    int mInitialWorkerCount;
    float mInitialMinerals;
    float mInitialVespeneGas;

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
                configFile >> mInitialWorkerCount;
            }
            else if(currentString.compare("Minerals") == 0)
            {
                configFile >> mInitialMinerals;
            }
            else if(currentString.compare("VespeneGas") == 0)
            {
                configFile >> mInitialVespeneGas;
            }
        }

    }

    inline int getInitialWorkerCount()
    {
        return mInitialWorkerCount;
    }

    inline float getInitialMinerals()
    {
        return mInitialMinerals;
    }

    inline float getInitialVespeneGas()
    {
        return mInitialVespeneGas;
    }

};
#endif // STARTINGCONFIGURATION_H

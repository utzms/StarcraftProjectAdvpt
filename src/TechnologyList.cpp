#include <TechnologyList.h>

TechnologyList::TechnologyList()
{
}

void TechnologyList::initUnitList(std::string race)
{
	//concat string to equal valid path and open its File
	std::string file = "./data/"+race+"/units.txt";
	DataReader readFile(file);
	//using boost lib as strtok equivalent
	typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
	std::string toBeTokenized;
	//separator Tokens = "\t"
	//drop empty strings (e.g. "\t\t", in between an empty string)
	boost::char_separator<char> separatorTokens("\t","", boost::drop_empty_tokens);
	//requirements are currently separated via ","
	boost::char_separator<char> requireTokens(",","", boost::drop_empty_tokens);
	while (!(toBeTokenized=readFile.getLine()).empty())
	{
		std::string unitName="";
		float unitCostMinerals=0.0f;
		float unitCostGas=0.0f;
		float unitCostSupply=0.0f;
		int unitBuildTime=0;
		std::string requirements;
		tokenizer tok(toBeTokenized, separatorTokens);
		//tok must hold 7 elements, otherwise it is no correct line
		if (std::distance(tok.begin(), tok.end()) == 7)
		{
			tokenizer::iterator tok_iter = tok.begin();
			if (*tok_iter == "Unit")
			{
				continue;
			} else
			{
				if (!(*tok_iter == "-"))
					unitName = *tok_iter;
				*tok_iter++;
				if (!(*tok_iter == "-"))
				{
					//stringstream for converting String to float/int
					std::stringstream stream;
					stream << *tok_iter;
					stream >> unitCostMinerals;
				}
				*tok_iter++;
				if (!(*tok_iter == "-"))
				{
					std::stringstream stream;
					stream << *tok_iter;
					stream >> unitCostGas;
				}
				*tok_iter++;
				if (!(*tok_iter == "-"))
				{
					std::stringstream stream;
					stream << *tok_iter;
					stream >> unitCostSupply;
				}
				*tok_iter++;
				if (!(*tok_iter == "-"))
				{
					std::stringstream stream;
					stream << *tok_iter;
					stream >> unitBuildTime;
				}
				*tok_iter++;
				units.insert( std::pair<std::string, Technology*>(unitName,
							new Technology(unitName,unitCostMinerals,unitCostGas,unitCostSupply,unitBuildTime)));
			}
		}
	}
}

Technology *TechnologyList::findUnit(std::string key)
{
	return NULL;
}


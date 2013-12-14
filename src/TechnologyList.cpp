#include "../include/TechnologyList.h"

TechnologyList::TechnologyList()
{
}
TechnologyList::TechnologyList(std::string unitPathIn, std::string buildPath)
{
	initialized=false;
	this->unitPath=unitPathIn;
	this->buildingPath=buildPath;

	initUnitList(unitPath);
	initBuildingList(buildingPath);
}

void TechnologyList::initRest()
{
#ifdef DEBUG
	std::cout << "INIT REST of TechnolgyList" << std::endl;
#endif
	std::string requirements;
	std::string vanish;
	std::string buildFrom;

	size_t from,fromfrom,to,toto;

	auto unitIt = unresolvedUnitRequirements.begin();
	for (; unitIt != unresolvedUnitRequirements.end(); ++unitIt)
	{
		std::vector<std::string> fixedRequirements;
		std::vector<std::vector<std::string>> orRequirements;

		std::shared_ptr<Technology> tech = unitIt->first;
		requirements = unitIt->second.req;
		vanish = unitIt->second.vanish;
		buildFrom = unitIt->second.buildFrom;
		//requirement check
		{
			from=0;
			to=0;
			std::string sub;
			do
			{
				to=requirements.find(",");
				sub = requirements.substr(from,to);
				//checken ob eine "eine von vielen" requirement ist
				if (sub.find("/")!=std::string::npos)
				{
					std::vector<std::string> orReq;
					std::string subsub;
					toto=0;
					fromfrom=0;
					do
					{
						toto=sub.find("/");
						subsub=sub.substr(fromfrom,toto);
						orReq.push_back(subsub);
						sub=sub.substr(toto+1,std::string::npos);
					} while(toto != std::string::npos);
					orRequirements.push_back(orReq);
				} else
				{
					if (sub != "0")
						fixedRequirements.push_back(sub);
				}
				requirements=requirements.substr(to+1,std::string::npos);
			} while (to!=std::string::npos);
			//add requirements to Technology
			for (size_t i = 0; i < fixedRequirements.size(); ++i)
			{
				std::shared_ptr<Technology> source;
                std::pair<std::shared_ptr<Technology>, RequirementType> commit;
                commit.second = RequirementType::Existence;
				if ((source=findUnit(fixedRequirements[i]))==NULL)
				{
					//its a building
					if ((source=findBuilding(fixedRequirements[i]))==NULL)
					{
						std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
					} else
					{
						commit.first = source;
						tech->addRequirement(commit);
					}
				} else
				{
					commit.first = source;
					tech->addRequirement(commit);
				}
			}
			for (size_t i = 0; i < orRequirements.size(); ++i)
			{
                std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
				for (size_t j = 0; j < orRequirements[i].size(); ++j)
				{
					std::shared_ptr<Technology> source;
                    std::pair<std::shared_ptr<Technology>,RequirementType> commit;
                    commit.second = RequirementType::Existence;
					if ((source=findUnit(orRequirements[i][j]))==NULL)
					{
						//its a building
						if ((source=findBuilding(orRequirements[i][j]))==NULL)
						{
							std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
						} else
						{
							commit.first = source;
							sources.push_back(commit);
						}
					} else
					{
						commit.first = source;
						sources.push_back(commit);
					}
				}
				tech->addRequirement(sources);
			}
			orRequirements.clear();
			fixedRequirements.clear();
		}
		/*
		 * Build From Check:
		 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		 * Care, using fixed- and orRequirements, dont mix it, because of the name
		 */
		{
			from=0;
			to=0;
			std::string sub;
			std::vector<std::string> orReq;
			do
			{
				to=buildFrom.find(",");
				sub=buildFrom.substr(from,to);
				if (sub != "0")
					orReq.push_back(sub);
				buildFrom=buildFrom.substr(to+1,std::string::npos);
			} while (to!=std::string::npos);
			orRequirements.push_back(orReq);
			for (size_t i = 0; i < orRequirements.size(); ++i)
			{
                std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;

				for (size_t j = 0; j < orRequirements[i].size(); ++j)
				{
					std::shared_ptr<Technology> source;
                    std::pair<std::shared_ptr<Technology>,RequirementType> commit;

                    commit.second = RequirementType::ForProduction;

                    if ((source = findUnit(orRequirements[i][j]))==NULL)
					{
						//its a building
						if ((source=findBuilding(orRequirements[i][j]))==NULL)
						{
							std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
						} else
						{
							commit.first = source;
							sources.push_back(commit);
						}
					} else
					{
						commit.first = source;
						sources.push_back(commit);
					}
				}
				tech->addRequirement(sources);
			}
			orRequirements.clear();
			fixedRequirements.clear();
		} //end buildFrom check
		/*
		 * Vanish Check:
		 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		 * Care, using fixed- and orRequirements, dont mix it, because of the name
		 */
		{
			from=0;
			to=0;
			std::string sub;
			std::vector<std::string> orReq;
			do
			{
				to=vanish.find(",");
				sub=vanish.substr(from,to);
				if (sub != "0")
					orReq.push_back(sub);
				vanish=vanish.substr(to+1,std::string::npos);
			} while (to!=std::string::npos);
			orRequirements.push_back(orReq);
			for (size_t i = 0; i < orRequirements.size(); ++i)
			{
                std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
				for (size_t j = 0; j < orRequirements[i].size(); ++j)
				{
					std::shared_ptr<Technology> source;
                    std::pair<std::shared_ptr<Technology>,RequirementType> commit;

                    commit.second = RequirementType::Vanishing;

					if ((source=findUnit(orRequirements[i][j]))==NULL)
					{
						//its a building
						if ((source=findBuilding(orRequirements[i][j]))==NULL)
						{
							std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
						} else
						{
							commit.first = source;
							sources.push_back(commit);
						}
					} else
					{
						commit.first = source;
						sources.push_back(commit);
					}
				}
				tech->addRequirement(sources);
			}
			orRequirements.clear();
			fixedRequirements.clear();
		} //end buildFrom check
	} //end unit it for loop


	//here starts the building list
	auto buildIt = unresolvedBuildingRequirements.begin();
	for (; buildIt != unresolvedBuildingRequirements.end(); ++buildIt)
	{
		std::vector<std::string> fixedRequirements;
		std::vector<std::vector<std::string>> orRequirements;
		std::shared_ptr<Technology> tech = buildIt->first;
		requirements = buildIt->second.req;
		vanish = buildIt->second.vanish;
		buildFrom = buildIt->second.buildFrom;
		//requirement check
		{
			from=0;
			to=0;
			std::string sub;
			do
			{
				to=requirements.find(",");
				sub = requirements.substr(from,to);
				//checken ob eine "eine von vielen" requirement ist
				if (sub.find("/")!=std::string::npos)
				{
					std::vector<std::string> orReq;
					std::string subsub;
					toto=0;
					fromfrom=0;
					do
					{
						toto=sub.find("/");
						subsub=sub.substr(fromfrom,toto);
						orReq.push_back(subsub);
						sub=sub.substr(toto+1,std::string::npos);
					} while(toto != std::string::npos);
					orRequirements.push_back(orReq);
				} else
				{
					if (sub != "0")
						fixedRequirements.push_back(sub);
				}
				if (to != std::string::npos)
					requirements=requirements.substr(to+1,std::string::npos);
			} while (to!=std::string::npos);
			//add requirements to Technology
			for (size_t i = 0; i < fixedRequirements.size(); ++i)
			{
				std::shared_ptr<Technology> source;
                std::pair<std::shared_ptr<Technology>,RequirementType> commit;
                commit.second = RequirementType::Existence;
				if ((source=findUnit(fixedRequirements[i]))==NULL)
				{
					//its a building
					if ((source=findBuilding(fixedRequirements[i]))==NULL)
					{
						std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
					} else
					{
						commit.first = source;
						tech->addRequirement(commit);
					}
				} else
				{
					commit.first = source;
					tech->addRequirement(commit);
				}
			}
		}
		for (size_t i = 0; i < orRequirements.size(); ++i)
		{
            std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
			for (size_t j = 0; j < orRequirements[i].size(); ++j)
			{
				std::shared_ptr<Technology> source;
                std::pair<std::shared_ptr<Technology>,RequirementType> commit;
                commit.second = RequirementType::Existence;
				if ((source=findUnit(orRequirements[i][j]))==NULL)
				{
					//its a building
					if ((source=findBuilding(orRequirements[i][j]))==NULL)
					{
						std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
					} else
					{
						commit.first = source;
						sources.push_back(commit);
					}
				} else
				{
					commit.first = source;
					sources.push_back(commit);
				}
			}
			tech->addRequirement(sources);
		}
			orRequirements.clear();
			fixedRequirements.clear();
		/*
		 * Vanish Check:
		 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		 * Care, using fixed- and orRequirements, dont mix it, because of the name
		 */
		{
			from=0;
			to=0;
			std::string sub;
			std::vector<std::string> orReq;
			do
			{
				to=vanish.find(",");
				sub=vanish.substr(from,to);
				//checken ob eine "eine von vielen" requirement ist
				if (sub != "0")
					orReq.push_back(sub);
				vanish=vanish.substr(to+1,std::string::npos);
			} while (to!=std::string::npos);
			orRequirements.push_back(orReq);
			for (size_t i = 0; i < orRequirements.size(); ++i)
			{
                std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>> sources;
				for (size_t j = 0; j < orRequirements[i].size(); ++j)
				{
					std::shared_ptr<Technology> source;
                    std::pair<std::shared_ptr<Technology>,RequirementType> commit;
                    commit.second = RequirementType::Vanishing;
					if ((source=findUnit(orRequirements[i][j]))==NULL)
					{
						//its a building
						if ((source=findBuilding(orRequirements[i][j]))==NULL)
						{
							std::cerr << "Fatal error: No instance of Technology: " << fixedRequirements[i] << std::endl;
						} else
						{
							commit.first = source;
							sources.push_back(commit);
						}
					} else
					{
						commit.first = source;
						sources.push_back(commit);
					}
				}
				tech->addRequirement(sources);
			}
			orRequirements.clear();
			fixedRequirements.clear();
		} //end vanish check
	}
}

void TechnologyList::initUnitList(std::string unitList)
{
	DataReader readFile(unitList);
	std::string line;
	bool callRest=initialized;
	while (!((line=readFile.getLine()).empty()))
	{
		initialized=true;
		std::string unitName="";
		float unitCostMinerals=0.0f;
		float unitCostGas=0.0f;
		float unitCostSupply=0.0f;
		int unitBuildTime=0;
		std::string requirements;
		std::string buildFrom;
		std::string vanish;
		std::stringstream stream;

		stream << line;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitName;
		if (unitName == "Unit")
			continue;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostMinerals;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostGas;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitCostSupply;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> unitBuildTime;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildFrom;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> requirements;
		if (!(stream.good()))
		{
			std::cerr << "INIT UNIT LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> vanish;

		Technology *t = new Technology(unitName,unitCostMinerals,unitCostGas,unitCostSupply,unitBuildTime);
		std::shared_ptr<Technology> tech(t);

		units.insert( std::pair<std::string, std::shared_ptr<Technology>>(unitName,
					tech));

		struct requirementRest rest;
		rest.req=requirements;
		rest.vanish=vanish;
		rest.buildFrom=buildFrom;

		auto unresolve = std::pair<std::shared_ptr<Technology>, requirementRest>(tech,rest);

		unresolvedUnitRequirements.insert(unresolve);

	}
	if (callRest)
		initRest();
}

void TechnologyList::initBuildingList(std::string buildList)
{
	DataReader readFile(buildList);
	std::string line;
	bool callRest=initialized;
	while (!((line=readFile.getLine()).empty()))
	{
		initialized=true;
		std::string buildName="";
		float buildCostMinerals=0.0f;
		float buildCostGas=0.0f;
		float buildCostSupply=0.0f;
		int buildBuildTime=0;
		std::string vanish;
		std::string requirements;
		std::stringstream stream;

		stream << line;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildName;
		if (buildName == "Building")
		{
			continue;
		}

		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostMinerals;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostGas;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildBuildTime;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> buildCostSupply;

		Technology *t = new Technology(buildName,buildCostMinerals,buildCostGas,buildCostSupply,buildBuildTime);
		std::shared_ptr<Technology> tech(t);
		buildings.insert( std::pair<std::string, std::shared_ptr<Technology>>(buildName,
					tech));

		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> requirements;
		if (!(stream.good()))
		{
			std::cerr << "INIT BUILDING LIST : stream not good, probably no valid line"<<std::endl;
			continue;
		}
		stream >> vanish;

		struct requirementRest rest;
		rest.req=requirements;
		rest.vanish=vanish;

		auto unresolve = std::pair<std::shared_ptr<Technology>, requirementRest>(tech,rest);

		unresolvedBuildingRequirements.insert(unresolve);

	}
	if (callRest)
		initRest();
}

void TechnologyList::reset()
{
	for (auto it : units)
	{
		(it).second->setExistence(false);
	}
	for (auto it : buildings)
	{
		(it).second->setExistence(false);
	}
}

bool TechnologyList::isInitialized()
{
	return initialized;
}

std::vector<std::vector<std::shared_ptr<Technology>>> TechnologyList::getPassedRequirements()
{
	return passedRequirements;
}

bool TechnologyList::checkRequirements(std::vector<std::vector<std::pair<std::shared_ptr<Technology>,RequirementType>>> req)
{
	std::vector<std::shared_ptr<Technology>> missing;
	std::vector<std::vector<std::shared_ptr<Technology>>> pass;
	bool allOK = true;
	std::string out="";
	for (size_t outer = 0; outer < req.size(); ++outer)
	{
		std::vector<std::shared_ptr<Technology>> inPass;
		bool ok=false;
		for (size_t inner = 0; inner < req[outer].size(); ++inner)
		{
			if (req[outer][inner].first->exists()==true)
			{
				inPass.push_back(req[outer][inner].first);
				ok=true;
				break;
			} else
			{
				missing.push_back(req[outer][inner].first);
			}
		}
		if (ok==false)
		{
			if (allOK == true)
			{
				out += "Missing requirements:\t";
				allOK = false;
			}
			for (size_t miss = 0; miss < missing.size(); ++miss)
			{
				out+= missing[miss]->getName();
				if (miss != missing.size()-1)
					out+= "/";
			}
			out+=",";
			missing.clear();
		} else
		{
			pass.push_back(inPass);
		}
		inPass.clear();
	}
	if (allOK == false)
	{
		passedRequirements.clear();
		out.pop_back();
		std::cerr << out;
		std::cerr << std::endl;
		return false;
	}
	passedRequirements=pass;
	return true;
}

bool TechnologyList::isBuildListPossible(std::vector<std::string> wuff)
{
	//currently implemented vor Protoss only
	//Starting buildings/units have to be called by outer functions earlier!
	auto initBuild = findBuildingVec("Nexus");
	auto initUnit = findUnitVec("Probe");
	for (size_t tmp=0; tmp < initBuild.size(); ++tmp)
	{
		initBuild[tmp]->setExistence(true);
	}
	for (size_t tmp=0; tmp < initUnit.size(); ++tmp)
	{
		initUnit[tmp]->setExistence(true);
	}
	for (size_t i = 0; i < wuff.size(); ++i)
	{
		auto testPossible = findUnitVec(wuff[i]);
		if (testPossible.empty())
		{
			testPossible = findBuildingVec(wuff[i]);
		}
		if (testPossible.empty())
		{
			std::cerr << "FATAL ERROR: Couldnt find Technology: " << wuff[i] << std::endl;
			return false;
		}
		for (size_t vec=0; vec < testPossible.size(); ++vec)
		{
			std::string progress = "Checking: " + testPossible[vec]->getName();
			PROGRESS(progress);
			if (checkRequirements(testPossible[vec]->getRequirements()) == false)
			{
				return false;
			} else
			{
				testPossible[vec]->setExistence(true);
			}
		}
	}
	//when finished, call reset()
	//either here or after the calling function
	return true;
}

std::vector<std::shared_ptr<Technology>> TechnologyList::findUnitVec(std::string key)
{
	std::vector<std::shared_ptr<Technology>> ret;
	auto it = units.equal_range(key).first;
	for (;it != units.equal_range(key).second;++it)
	{
		ret.push_back((*it).second);
	}
	return ret;
}
std::vector<std::shared_ptr<Technology>> TechnologyList::findBuildingVec(std::string key)
{
	std::vector<std::shared_ptr<Technology>> ret;
	auto it = buildings.equal_range(key).first;
	for (;it != buildings.equal_range(key).second;++it)
	{   
		ret.push_back((*it).second);
	}
	return ret;
}

void TechnologyList::printAll()
{
	for(auto it : units)
	{
		std::cout << (it).second << std::endl;
	}
	std::cout << std::endl;
	for(auto it : buildings)
	{
		std::cout << (it).second << std::endl;
	}
}


std::shared_ptr<Technology> TechnologyList::findBuilding(std::string key)
{
	static std::string lastKey="PlaceHolder";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = buildings.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG
		std::cerr << "Nothing found in building list with key:\t" << key << std::endl;
#endif
		return NULL;
	} else
	{
		if (lastKey == key)
		{
			++it;
			if (it != buildings.equal_range(key).second)
			{
				return (*it).second;
			} else
			{
#ifdef DEBUG
				std::cout << "No other building with key:\t" << key << " found" << std::endl;
#endif
				lastKey = "PlaceHolder";
				return findBuilding(key);
			}
		} else
		{
			lastKey=key;
			it = buildings.equal_range(key).first;
			return (*it).second;
		}
	}
}

std::shared_ptr<Technology> TechnologyList::findUnit(std::string key)
{
	static std::string lastKey="PlaceHolder";
	static std::multimap<std::string,std::shared_ptr<Technology>>::iterator it;

	int count = units.count(key);
	if (count == 0)
	{
		lastKey="PlaceHolder";
#ifdef DEBUG
		std::cerr << "Nothing found in unit list with key:\t" << key << std::endl;
#endif
		return NULL;
	} else
	{
		if (lastKey == key)
		{
			++it;
			if (it != units.equal_range(key).second)
			{
				return (*it).second;
			} else
			{
#ifdef DEBUG
				std::cout << "No other unit with key:\t" << key << " found\t" << count << std::endl;
#endif
				lastKey = "PlaceHolder";
				return findUnit(key);
			}
		} else
		{
			lastKey=key;
			it = units.equal_range(key).first;
			return (*it).second;
		}
	}
}


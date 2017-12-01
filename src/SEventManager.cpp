
#include "SEventManager.hpp"

SEventManager::SEventManager() {}

SEventManager::~SEventManager() {}

SEventManager &	SEventManager::getInstance()
{
    static SEventManager    instance;
    return instance;
}


void		SEventManager::registerEvent(Event::Enum event, void (*f)(void*))
{
	std::map<Event::Enum, std::vector<void(*)(void*)>>::iterator	it;
	it = _map.find(event);

	if (it != _map.end())
		it->second.push_back(f);
	else
	{
		std::vector<void(*)(void*)>	vec;
		vec.push_back(f);
		_map.insert(std::pair<Event::Enum, std::vector<void(*)(void*)>>(event, vec));
	}
}


void		SEventManager::raise(Event::Enum event, void* param)
{
	std::map<Event::Enum, std::vector<void(*)(void*)>>::iterator	it;
	it = _map.find(event);

	if (it != _map.end())
	{
		for (auto itVec = it->second.begin() ; itVec != it->second.end() ; itVec++)
			(*itVec)(param);
	}	
}
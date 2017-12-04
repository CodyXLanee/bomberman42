
#include "SEventManager.hpp"

SEventManager::SEventManager() {}

SEventManager::~SEventManager() {}

SEventManager &	SEventManager::getInstance()
{
    static SEventManager    instance;
    return instance;
}


void		SEventManager::registerEvent(Event::Enum event, CallbackType f)
{
	std::map<Event::Enum, std::vector<CallbackType>>::iterator	it;
	it = _map.find(event);

	if (it != _map.end())
		it->second.push_back(f);
	else
	{
		std::vector<CallbackType>	vec;
		vec.push_back(f);
		_map.insert(std::pair<Event::Enum, std::vector<CallbackType>>(event, vec));
	}
}


void		SEventManager::raise(Event::Enum event, void* param)
{
	std::map<Event::Enum, std::vector<CallbackType>>::iterator	it;
	it = _map.find(event);

	if (it != _map.end())
	{
		for (auto itVec = it->second.begin() ; itVec != it->second.end() ; itVec++)
			(*itVec)(param);
	}	
}
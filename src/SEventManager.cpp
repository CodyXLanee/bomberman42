
#include "SEventManager.hpp"

SEventManager::SEventManager() {}

SEventManager::~SEventManager() {}

SEventManager &	SEventManager::getInstance()
{
    static SEventManager    instance;
    return instance;
}


void		SEventManager::registerEvent(Event::Enum event, std::pair<CallbackType, void*> callback_and_author)
{
	auto it = _map.find(event);

	if (it != _map.end())
		it->second.push_back(callback_and_author);
	else
	{
		std::vector<std::pair<CallbackType, void*>>	vec;
		vec.push_back(callback_and_author);
		_map.insert(std::pair<Event::Enum, std::vector<std::pair<CallbackType, void*>>>(event, vec));
	}
}

void		SEventManager::unRegisterEvent(Event::Enum event, void *author)
{
	auto it = _map.find(event);

	if (it != _map.end()){
		it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [&author](std::pair<CallbackType, void*> pair){
			return author == pair.second;
		}), it->second.end());
	}

}


void		SEventManager::raise(Event::Enum event, void* param)
{
	auto it = _map.find(event);

	if (it != _map.end())
	{
		for (auto itVec = it->second.begin() ; itVec != it->second.end() ; itVec++)
			(*itVec).first(param);
	}	
}
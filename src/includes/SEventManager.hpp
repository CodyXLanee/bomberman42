
#ifndef SEVENTMANAGER_HPP
# define SEVENTMANAGER_HPP

# include <iostream>
# include <map>
# include <vector>

namespace Event {
	enum Enum	{
		KEYPRESS
	};
}

class SEventManager {
    public:
    	~SEventManager();
        static SEventManager& getInstance();
		void	registerEvent(Event::Enum event, void (*f)(void*));
		void	raise(Event::Enum event, void*);

    private:
		std::map<Event::Enum, std::vector<void(*)(void*)>> _map;
        SEventManager();
        SEventManager(SEventManager const&);              // Don't Implement.
        void operator=(SEventManager const&); // Don't implement
	
};

#endif
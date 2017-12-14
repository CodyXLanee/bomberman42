
#ifndef SEVENTMANAGER_HPP
# define SEVENTMANAGER_HPP

# include <iostream>
# include <map>
# include <vector>

typedef std::function<void(void*)> CallbackType;

#define MEMBER_CALLBACK(funPtr) \
std::pair<CallbackType, void*>(std::bind(&funPtr, this, std::placeholders::_1), this)

#define MEMBER_CALLBACK_WITH_INSTANCE(funPtr, instancePtr) \
std::pair<CallbackType, void*>(std::bind(&funPtr, instancePtr, std::placeholders::_1), instancePtr)


namespace Event {
	enum Enum	{
		KEYDOWN, KEYUP,

		PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN,
		END_PLAYER_LEFT, END_PLAYER_RIGHT, END_PLAYER_UP, END_PLAYER_DOWN,
		
		HUMAN_PLAYER_LEFT, HUMAN_PLAYER_RIGHT, HUMAN_PLAYER_UP, HUMAN_PLAYER_DOWN,
		END_HUMAN_PLAYER_LEFT, END_HUMAN_PLAYER_RIGHT, END_HUMAN_PLAYER_UP, END_HUMAN_PLAYER_DOWN,

		HUMAN_SPAWN_BOMB, END_HUMAN_SPAWN_BOMB,
		SPAWN_BOMB, SPAWN_FLAME,
		BOMB_EXPLODES, FLAME_DISAPEAR,

		BRICK_BREAKS,

		PLAYER_DIES, ENEMY_DIES,

		BONUS_ACTIVATE,

		UI_AUDIO,

		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN, RESET_CAMERA, FOLLOW_PLAYER, DEBUG_MODE, CAMERA_MODE_UPDATE,
		GUI_TOGGLE, GUI_BASE_MENU,
		MASTER_VOLUME_UPDATE, MUSIC_VOLUME_UPDATE, EFFECTS_VOLUME_UPDATE,
		SCREEN_FORMAT_UPDATE,

		NEW_GAME, LOAD_GAME, QUIT_GAME,
		GAME_WIN, GAME_FINISH,
		
		PLAYER_MOVE, ENEMY_MOVE,

		ENEMY_COLLIDES,

		AIPTR
	};
}

class SEventManager {
    public:
    	~SEventManager();
		static SEventManager& getInstance();

		void	registerEvent(Event::Enum event, std::pair<CallbackType, void*> callback_and_author);
		void	unRegisterEvent(Event::Enum event, void *author);
		void	raise(Event::Enum event, void*);

	private:
		std::map<Event::Enum, std::vector<std::pair<CallbackType, void*>>> _map;

        SEventManager();
        SEventManager(SEventManager const&);              // Don't Implement.
        void operator=(SEventManager const&); // Don't implement
	
};

#endif
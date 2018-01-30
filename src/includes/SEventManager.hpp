
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
/*		SDL_Keycode																						*/
		KEYDOWN, 	KEYUP,

/*		Animation::Enum *																				*/
		START_ANIMATION, END_ANIMATION, 

/*		Player *																						*/
		PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN,
/*		Player *																						*/
		END_PLAYER_LEFT, END_PLAYER_RIGHT, END_PLAYER_UP, END_PLAYER_DOWN,

/*		nullptr																							*/
		HUMAN_PLAYER_LEFT, HUMAN_PLAYER_RIGHT, HUMAN_PLAYER_UP, HUMAN_PLAYER_DOWN,
/*		nullptr																							*/
		END_HUMAN_PLAYER_LEFT, END_HUMAN_PLAYER_RIGHT, END_HUMAN_PLAYER_UP, END_HUMAN_PLAYER_DOWN,
/*		nullptr											*/
		HUMAN_SPAWN_BOMB, END_HUMAN_SPAWN_BOMB,
/*		Player *  		 | Flame *	  |	Bomb *			*/
		PLAYER_SPAWN_BOMB, SPAWN_FLAME, SPAWN_BOMB,

/*		Bomb *		 | Flame *							*/
		BOMB_EXPLODES, FLAME_DISAPEAR,
/*		glm::vec2 *										*/
		BRICK_BREAKS,
/*		Player *   | Enemy *							*/
		PLAYER_DIES, ENEMY_DIES,

/*		std::pair<Bonus *, Player *> * 					*/
		BONUS_ACTIVATE,
/*		UIAudio::Enum *									*/
		UI_AUDIO,

/*		nulptr 																		  | not raised|	Camera::Mode *					*/
		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN, RESET_CAMERA, FOLLOW_PLAYER, DEBUG_MODE, CAMERA_MODE_UPDATE,
/*		Menu::Enum *| not raised																									*/
		GUI_TOGGLE	, GUI_BASE_MENU,
/*		float *				| float *			 | float *																	    	*/
		MASTER_VOLUME_UPDATE, MUSIC_VOLUME_UPDATE, EFFECTS_VOLUME_UPDATE,
/*		Screen::Format  *	| std::map<Event::Enum, SDL_Keycode>  *																	*/
		SCREEN_FORMAT_UPDATE, KEY_MAP_UPDATE,
/*		std::vector<int> *																											*/
		UPDATE_ALL_CAMPAIGN_STARS,

/*		Save::Enum *									*/
		LOAD_SLOT,
/*		GameParams * | nullptr  | nullptr				*/
		NEW_GAME,      QUIT_GAME, RESTART_GAME,
/*		nullptr | nullptr  | nullptr					*/
		GAME_WIN, GAME_OVER, GAME_FINISH,

/*		nullptr, nullptr		| nullptr				*/
		GAME_PAUSE, GAME_UNPAUSE, END_END_ANIMATION,

/*		Player *   | Enemy *							*/
		PLAYER_MOVE, ENEMY_MOVE,

/*		Enemy *											*/
		ENEMY_COLLIDES,

/* 		std::vector<glm::vec2> * 						*/
		AIPTR,
/*		Theme::Enum										*/
		SET_THEME
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
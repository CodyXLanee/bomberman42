/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 14:36:37 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/17 14:06:26 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"
#include "SEventManager.hpp"

void            SGameManager::useSlot(void) const {
    SEventManager &em = SEventManager::getInstance();
    Screen::Format  *sf = new Screen::Format(_slot->get_screenFormat());
    std::map<Event::Enum, SDL_Keycode>  *km = new std::map<Event::Enum, SDL_Keycode>(_slot->getKeyMap());

    em.raise(Event::SCREEN_FORMAT_UPDATE, sf);
    delete sf;

    em.raise(Event::KEY_MAP_UPDATE, km);
    delete km;

    float   f;
    f = _slot->get_master_volume();
    em.raise(Event::MASTER_VOLUME_UPDATE, &f);

    f = _slot->get_music_volume();
    em.raise(Event::MUSIC_VOLUME_UPDATE, &f);
    
    f = _slot->get_effects_volume();
    em.raise(Event::EFFECTS_VOLUME_UPDATE, &f);
    


}

SGameManager::SGameManager() : 
    _window(1920, 1080), 
    _camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f)), 
    _gui(_window, _camera), 
    _renderer(_window.getWin(), _camera),
    _slot(new Slot(Save::SLOT1)),
    _dev_mode(false),                                                   // <---- DEV MODE !
    _game_is_active(false), _quit_game(false), _new_game(false){
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::QUIT_GAME, MEMBER_CALLBACK(SGameManager::quit_game));
    em.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SGameManager::new_game));
    em.registerEvent(Event::GAME_FINISH, MEMBER_CALLBACK(SGameManager::game_finish));

    useSlot();
}

SGameManager::~SGameManager() {
    if (_slot){
        _slot->save();
    }
}

void        SGameManager::manage(void) {
    SEventManager &em = SEventManager::getInstance();
    if (!_dev_mode)
        em.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::START));
    else
        em.raise(Event::NEW_GAME, new GameMode::Enum(GameMode::CAMPAIGN));    
	while(!_quit_game) {
        if (_new_game)
            new_game(new GameMode::Enum(GameMode::CAMPAIGN));
            
		_window.eventManager(_gui.getContext());

        if (_game_is_active){
            _game->compute();
            
            _camera.update(_window.getMouseX(), _window.getMouseY(), _game->getPlayerPos());
            _renderer.render(_game->getMap(), _game->getEntityList());
        }
		_gui.render();
		_window.initGL();
		SDL_GL_SwapWindow(_window.getWin());
	}
}


SGameManager &	SGameManager::getInstance(){
    static SGameManager gm;
    return gm;
}

void            SGameManager::quit_game(void *){
    _quit_game = true;
}

void            SGameManager::new_game(void *p){
    GameMode::Enum *gm = static_cast<GameMode::Enum *>(p);
    delete _game;
    _game = new GameEngine(*gm);
    _game_is_active = true;
    _new_game = false;
}

void            SGameManager::game_finish(void *){
    _new_game = true;
}

void            SGameManager::newGame(GameMode::Enum gm){
    (void)gm;
}
void            SGameManager::loadGame(Save::Enum s) {
    (void)s;
}
void            SGameManager::saveGame(Save::Enum s) {
    (void)s;
}
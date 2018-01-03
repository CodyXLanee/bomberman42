/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 14:36:37 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/03 14:18:55 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"
#include "SEventManager.hpp"

void            SGameManager::useSlot(void) {
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

    _gui.setStarsCampaign(_slot->get_all_stars_campaign());
}

void        SGameManager::loadSlot(void *s){
    Save::Enum slot = *static_cast<Save::Enum *>(s);
    _slot = new Slot(slot);
    useSlot();
}

SGameManager::SGameManager() : 
    _window(1600, 900),
    _game(nullptr),
    _camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f)), 
    _gui(_window, _camera), 
    _renderer(_window.getWin(), _camera),
    _slot(nullptr),
    _dev_mode(false),                                                   // <---- DEV MODE !
    _game_is_active(false), _quit_game(false), _new_game(false){
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::QUIT_GAME, MEMBER_CALLBACK(SGameManager::quit_game));
    em.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SGameManager::new_game));
    em.registerEvent(Event::GAME_FINISH, MEMBER_CALLBACK(SGameManager::game_finish));

    em.registerEvent(Event::LOAD_SLOT, MEMBER_CALLBACK(SGameManager::loadSlot));
}

SGameManager::~SGameManager() {
    if (_slot){
        _slot->save();
    }
}

void        SGameManager::manage(void) {
    SEventManager &em = SEventManager::getInstance();
    if (!_dev_mode)
        em.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::SELECT_SLOT));
    else
        em.raise(Event::NEW_GAME, new GameMode::Enum(GameMode::CAMPAIGN));
//	while(1) {
        while (!_quit_game)
        {
            if (_new_game)
                new_game(new GameMode::Enum(GameMode::CAMPAIGN));
                
    		_window.eventManager(_gui.getContext());

            if (_game_is_active){
                _game->compute();
                
                _camera.update(_window.getMouseX(), _window.getMouseY(), _game->getPlayerPos());
                _renderer.render(_game->getMap(), _game->getEntityList());
            }
    		_gui.render(_game_is_active);
    		_window.initGL();
    		SDL_GL_SwapWindow(_window.getWin());
        }

        if (_game && _game->getWin() && _game->getGameParams().get_game_mode() == GameMode::CAMPAIGN)
        {
            int stars = _game->getStarsCampaign();
            if (stars != -1 && stars > _slot->get_stars_campaign(_game->getGameParams().get_level()))
                _slot->set_stars_campaign(_game->getGameParams().get_level(), stars);
        }
        // _gui.render(true);
        // em.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::NONE));
//	}
}


SGameManager &	SGameManager::getInstance(){
    static SGameManager gm;
    return gm;
}

void            SGameManager::quit_game(void *){
    _quit_game = true;
}

void            SGameManager::new_game(void *p){
    GameParams *gp = static_cast<GameParams *>(p);
    if (_game != nullptr)
        delete _game;
    _game = new GameEngine(*gp);
    _game_is_active = true;
    _new_game = false;
}

void            SGameManager::game_finish(void *){
    _game_is_active = false;
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
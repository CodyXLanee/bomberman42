/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 14:36:37 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/05 20:30:56 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"
#include "SEventManager.hpp"

SGameManager::SGameManager() : 
    _window(1920, 1080), 
    _camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f)), 
    _gui(_window, _camera), 
    _renderer(_window.getWin(), _camera),
    _game_is_active(true), _quit_game(false) {
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::QUIT_GAME, MEMBER_CALLBACK(SGameManager::quit_game));
    em.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SGameManager::new_game));
    em.registerEvent(Event::GAME_FINISH, MEMBER_CALLBACK(SGameManager::game_finish));
}


SGameManager::~SGameManager() {
}

void        SGameManager::manage(void) {
	std::vector<Action::Enum> actions;

    SEventManager &em = SEventManager::getInstance();
    // em.raise(Event::TOGGLE, new Menu::Enum(Menu::START));
    em.raise(Event::NEW_GAME, new GameMode::Enum(GameMode::CAMPAIGN));    
	while(!_quit_game && (actions.size() == 0 || actions[0] != Action::ESCAPE)) {
		_window.eventManager(actions, _gui.getContext());

        if (_game_is_active){
            _game->compute(actions);
            
            _camera.update(actions, _window.getMouseX(), _window.getMouseY(), _game->getPlayerPos());
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

void            SGameManager::quit_game(void *p){
    (void)p;
    _quit_game = true;
}

void            SGameManager::new_game(void *p){
    GameMode::Enum *gm = static_cast<GameMode::Enum *>(p);
    _game = new GameEngine(*gm);
    _game_is_active = true;
    delete gm;
}

void            SGameManager::game_finish(void *p){
    (void)p;
    new_game(new GameMode::Enum(GameMode::CAMPAIGN));
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
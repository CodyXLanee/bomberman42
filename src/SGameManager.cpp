/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 14:36:37 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/04 14:59:07 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"

SGameManager::SGameManager() : 
    _window(1920, 1080), 
    _game(), 
    _camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f)), 
    _gui(_window.getWin(), _camera), 
    _renderer(_window.getWin(), _camera){
}


SGameManager::~SGameManager() {
}

void        SGameManager::manage(void) {
	std::vector<Action::Enum> actions;

	while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
		_window.eventManager(actions, _gui);

		_game.compute(actions);
		
		_camera.update(actions, _window.getMouseX(), _window.getMouseY(), _game.getPlayerPos());
		_renderer.render(_game.getMap(), _game.getEntityList());
		_window.initGL();
		SDL_GL_SwapWindow(_window.getWin());
	}
}


SGameManager &	SGameManager::getInstance(){
    static SGameManager gm;
    return gm;
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
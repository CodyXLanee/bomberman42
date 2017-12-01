/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:32:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/01 13:27:46 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bomberman.hpp"
#include "Sdl_gl_win.hpp"
#include "GameEngine.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"

int main() {
	Sdl_gl_win		window(1920, 1080);
	GameEngine		game;
	Camera			camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f));
	RenderEngine	renderer(window.getWin(), camera);

	std::vector<Action::Enum> actions;

	while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
		window.eventManager(actions, renderer.getGUI());

		game.compute(actions);
		
		camera.update(actions, window.getMouseX(), window.getMouseY(), game.getPlayerPos());
		renderer.render(game.getMap(), game.getEntityList());
		renderer.renderGUI(actions);
		window.initGL();
		SDL_GL_SwapWindow(window.getWin());
	}
	
	return 0;
}
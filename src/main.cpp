/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:32:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 14:41:01 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bomberman.hpp"
#include "Sdl_gl_win.hpp"
#include "GameEngine.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"

int main() {
	Sdl_gl_win		window;
	GameEngine		game;
	Camera			camera(glm::vec3(5.f, -5.f, 10.f), glm::vec3(5.f, 5.f, 0.f));
	RenderEngine	renderer(window.getWin(), camera);

	std::vector<Action::Enum> actions;

	while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
		window.eventManager(actions);

		game.compute(actions);
		
		camera.update(actions, window.getMouseX(), window.getMouseY());
		renderer.render(game.getMap(), game.getEntityList());
	}
	
	return 0;
}
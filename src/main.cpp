/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:32:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 16:08:16 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bomberman.hpp"
#include "Sdl_gl_win.hpp"

int main() {
	Sdl_gl_win window;
	std::vector<Action::Enum> actions;

	while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
		window.eventManager(actions);
	}
	return 0;
}
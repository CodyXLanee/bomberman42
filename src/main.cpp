/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:32:16 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/31 10:48:41 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"

int main(int, char **av) {
	if (strcmp(av[0], "./bomberman")) {
		std::cerr << "You must launch the game from local directory" << std::endl;
		return 1;
	}
	std::srand(std::time(0));
	SGameManager &gm = SGameManager::getInstance();
	gm.manage();
	return 0;
}
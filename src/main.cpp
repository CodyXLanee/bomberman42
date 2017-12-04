/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:32:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/04 14:50:13 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"

int main() {
	SGameManager &gm = SGameManager::getInstance();
	gm.manage();
	return 0;
}
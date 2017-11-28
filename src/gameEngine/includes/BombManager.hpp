/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:20:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/28 15:30:31 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBMANAGER_HPP
# define BOMBMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "Bomb.hpp"
# include <vector>
# include <iostream>

class BombManager {
	
	public:
		BombManager();
		~BombManager();

		void			update(std::vector<IGameEntity *> &entityList, std::vector<Action::Enum> const actions);
	private:
		bool	_spawned_bomb;
};

#endif
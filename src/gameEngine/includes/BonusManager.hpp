/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BonusManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 09:53:11 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/11 16:36:56 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUSMANAGER_HPP
# define BONUSMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "SEventManager.hpp"
# include "Bonus.hpp"
# include "Player.hpp"
# include <random>

class BonusManager {
	
	public:
		BonusManager(std::vector<IGameEntity *> *entityList);
		~BonusManager();

		void			update(void);
	private:
        void            bonusActivateCallback(void *);
        void            brickBreaksCallback(void *);

		std::vector<IGameEntity *>	*_entity_list;
		std::vector<Bonus *>		*_bonus_queue;
};

#endif
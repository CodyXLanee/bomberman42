/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EnemyManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 19:10:51 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/14 22:46:19 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMYMANAGER_HPP
# define ENEMYMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "SEventManager.hpp"
# include "Enemy.hpp"

class EnemyManager {
	
	public:
		EnemyManager(std::vector<IGameEntity *> *entityList);
		~EnemyManager();

		void			update(void);
	private:
		void            enemyCollidesCallback(void *entity);
		void            enemyDiesCallback(void *enemy);
		glm::vec2       updateDirectionBaloon(glm::vec2 direction);
		
		std::vector<IGameEntity *>	*_entity_list;
};

#endif
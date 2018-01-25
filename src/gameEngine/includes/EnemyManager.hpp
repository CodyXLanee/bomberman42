/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EnemyManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 19:10:51 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 11:56:28 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMYMANAGER_HPP
# define ENEMYMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "SEventManager.hpp"
# include "Enemy.hpp"
# include "Map.hpp"
# include "Bomb.hpp"

class EnemyManager {
	
	public:
		EnemyManager(std::vector<IGameEntity *> *entityList, Map *map);
		~EnemyManager();

		void			setMap(Map *map);
		Map				*getMap() const;

		void			update(void);

		EnemyManager(void);
		EnemyManager(EnemyManager const &);
		EnemyManager   &operator=(EnemyManager const &rhs);
	private:
		void            enemyCollidesCallback(void *entity);
		void            enemyMoveCallback(void *entity);
		void            enemyDiesCallback(void *enemy);
		glm::vec2       updateDirectionBaloon(glm::vec2 direction, glm::vec2 position);
		
		std::vector<IGameEntity *>	*_entity_list;
		Map							*_map;
};

#endif
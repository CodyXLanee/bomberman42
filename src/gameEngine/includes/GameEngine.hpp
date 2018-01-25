/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:11:53 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/23 11:47:36 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEENGINE_HPP
# define GAMEENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "CollisionsManager.hpp"
# include "BombManager.hpp"
# include "BonusManager.hpp"
# include "EnemyManager.hpp"
# include "PlayerManager.hpp"
# include "WinManager.hpp"
# include "Map.hpp"
# include "Loader.hpp"
# include "GameParams.hpp"
# include <glm/glm.hpp>
# include <vector>
# include <math.h>

class GameEngine {
	
	public:
		GameEngine(GameParams &gp);
		~GameEngine();
		
		void								compute();
		Map const &							getMap() const;
		std::vector<IGameEntity *> &		getEntityList();
		glm::vec2		 					*getPlayerPos(void) const;
		int 								getStarsCampaign(void);
		GameParams 							getGameParams(void) const;
		bool								getWin(void) const;

		void								setActive(bool b);

		void								gameWin(void *ptr);
		void								gameOver(void *);
		void								pause(void *);
		void								unPause(void *);

	private:
		GameEngine();		// not implemented;
		void								loadMap(const char *path);
		glm::vec2							placeBrawlAI(Player *human, int i);
		void								placeBrawlPlayers(PlayerColor::Enum _color);
		
		Map * 						_map;
		Loader						_loader;
		std::vector<IGameEntity *>	*_entityList;
		BonusManager				*_bonusManager;
		EnemyManager				*_enemyManager;
		CollisionsManager			_collisionsManager;
		BombManager					*_bombManager;
		PlayerManager				*_playerManager;
		GameParams					_gameParams;
		WinManager					*_winManager;
		bool						_win;
		bool						_active;

};

#endif
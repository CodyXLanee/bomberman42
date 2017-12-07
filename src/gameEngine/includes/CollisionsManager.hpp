#ifndef COLLISIONSMANAGER_HPP
# define COLLISIONSMANAGER_HPP

# define RPLAYER 0.3	// 0.1 <= x <= 0.5

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Map.hpp"
# include "SEventManager.hpp"
# include "Player.hpp"
# include <glm/glm.hpp>
# include <vector>
# include <math.h>

class CollisionsManager {
	public:
		CollisionsManager();
		~CollisionsManager();

		void			moves(Map const & map, std::vector<IGameEntity *> &entityList);
		static bool		hasObstacle(Map const & map, glm::vec2 v, std::vector<IGameEntity *> const &entityList,  IGameEntity const * entity);

	private:
		void		compute_player(Player *p);
		void		gestionBorderMap(glm::vec2 & pos, Map const & map);
		int			gestionNoSlipMove(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList);
		void		gestionSlipOneDirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList);
		void		gestionSlipBidirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map);
		static bool	collidesWithEntity(glm::vec2 &v, IGameEntity const * entity, std::vector<IGameEntity *> const &entityList);
		void		computePlayerMovement(Map const & map, std::vector<IGameEntity *> &entityList, IGameEntity *player);
};

#endif
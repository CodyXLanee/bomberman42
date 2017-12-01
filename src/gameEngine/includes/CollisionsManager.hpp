
#ifndef COLLISIONSMANAGER_HPP
# define COLLISIONSMANAGER_HPP

# define RPLAYER 0.3	// 0.1 <= x <= 0.5

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Map.hpp"
# include <glm/glm.hpp>
# include <vector>
# include <math.h>

class CollisionsManager {
	
	public:
		CollisionsManager();
		~CollisionsManager();

		void			moves(Map const & map, std::vector<IGameEntity *> &entityList, std::vector<Action::Enum> const actions);

	private:
		glm::vec2	compute_direction(std::vector<Action::Enum> actions);
		void		compute_player(IGameEntity*p, std::vector<Action::Enum> actions);
		void		gestionBorderMap(glm::vec2 & pos, Map const & map);
		int			gestionNoSlipMove(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList);
		void		gestionSlipOneDirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList);
		void		gestionSlipBidirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map);
		bool		hasObstacle(Map const & map, glm::vec2 v, std::vector<IGameEntity *> const &entityList,  IGameEntity const * entity);
		bool		collidesWithEntity(glm::vec2 &v, IGameEntity const * entity, std::vector<IGameEntity *> const &entityList);
};

#endif
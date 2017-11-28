
#ifndef COLLISIONSMANAGER_HPP
# define COLLISIONSMANAGER_HPP

# define RPLAYER 0.5	// 0.1 <= x <= 0.5

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

		void			moves(Map const & map, std::vector<IGameEntity *> entityList, std::vector<Action::Enum> const actions);

	private:
		glm::vec2	compute_direction(std::vector<Action::Enum> actions);
		void		compute_player(IGameEntity*p, std::vector<Action::Enum> actions);
		void		gestionBorderMap(glm::vec2 & pos, Map const & map);
		int			gestionNoSlipMove(glm::vec2 & pos, IGameEntity const * entity, Map const & map);
		void		gestionSlipOneDirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map);
		void		gestionSlipBidirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map);
		
};

#endif
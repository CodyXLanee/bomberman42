
#include "CollisionsManager.hpp"

CollisionsManager::CollisionsManager() {}

CollisionsManager::~CollisionsManager() {}

void			CollisionsManager::moves(Map const & map, std::vector<IGameEntity *> &entityList, std::vector<Action::Enum> const actions)
{

	for (std::vector<IGameEntity *>::iterator i = entityList.begin(); i != entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:
				compute_player(*i, actions);
				if ((*i)->getState() == State::MOVING){
					glm::vec2	newPos = (*i)->getPosition() + ((*i)->getDirection() * (*i)->getSpeed());
					
					gestionBorderMap(newPos, map);
					
					if (!gestionNoSlipMove(newPos, *i, map, entityList))
					{
						if ((*i)->getDirection().x == 0 || (*i)->getDirection().y == 0)
							gestionSlipOneDirection(newPos, *i, map, entityList);
						else
							gestionSlipBidirection(newPos, *i, map);
					}

					// gestion border

					// set the new position
					//(*i)->setDirection(normalize(newPos - (*i)->getPosition()));

					SEventManager::getInstance().raise(Event::PLAYER_MOVE, &newPos);
					(*i)->setPosition(newPos);
				}
				break;
			default:
				(void)actions;
				break;
		}
	}
}

glm::vec2	CollisionsManager::compute_direction(std::vector<Action::Enum> actions) {
	glm::vec2 v(0., 0.);
	for (std::vector<Action::Enum>::iterator i = actions.begin(); i != actions.end(); i++) {
		switch (*i) {
			case Action::LEFT: 		v.x += -1.; break;
			case Action::RIGHT: 	v.x += 1.; break;
			case Action::UP: 		v.y += 1.; break;
			case Action::DOWN: 		v.y += -1.; break;
			default:				(void)v;
		}
	}
	return (glm::normalize(v));
}

void	CollisionsManager::compute_player(IGameEntity *p, std::vector<Action::Enum> actions){
	glm::vec2	v = compute_direction(actions);
	(void)p;
	if (!std::isnan(v.x) && !std::isnan(v.y)){ // glm::isnan wouldn't compile for some reason...
		p->setState(State::MOVING);
		p->setDirection(v);
		p->setSpeed(0.1);
	}
	else
	{
		p->setState(State::STANDING);
		p->setSpeed(0.);
	}
}

void			CollisionsManager::gestionBorderMap(glm::vec2 & pos, Map const & map)
{
	if (pos.x < 0)
		pos.x = 0;
	if (pos.x > (map.getSize().x - 1))
		pos.x = map.getSize().x - 1;
	if (pos.y < 0)
		pos.y = 0;
	if (pos.y > (map.getSize().y - 1))
		pos.y = map.getSize().y - 1;
}

int				CollisionsManager::gestionNoSlipMove(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList)
{
	int	isBlocked = 0;

	if (entity->getDirection().x > 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x + RPLAYER), round(pos.y)), entityList, entity))
		{
			isBlocked = 1;
			pos.x = round(pos.x + RPLAYER) - RPLAYER - 0.5;
		}
	}
	if (entity->getDirection().x < 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x - RPLAYER), round(pos.y)), entityList, entity))
		{
			isBlocked = 1;
			pos.x = round(pos.x - RPLAYER) + RPLAYER + 0.5;
		}
	}
	if (entity->getDirection().y > 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x), round(pos.y + RPLAYER)), entityList, entity))
		{
			isBlocked = 1;
			pos.y = round(pos.y + RPLAYER) - RPLAYER - 0.5;
		}
	}
	if (entity->getDirection().y < 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x), round(pos.y - RPLAYER)), entityList, entity))
		{
			isBlocked = 1;
			pos.y = round(pos.y - RPLAYER) + RPLAYER + 0.5;
		}
	}

	return isBlocked;
}

bool			CollisionsManager::collidesWithEntity(glm::vec2 &v, IGameEntity const * entity, std::vector<IGameEntity *> const &entityList){
	for (auto i = entityList.begin(); i != entityList.end(); i++){
		if ((*i) != entity){
			switch((*i)->getType()){
				case Type::BOMB:
															//	this is so a player already on the same spot as an bomb doesn't produce a collision
					if (glm::round(v) == (*i)->getPosition() && glm::round(entity->getPosition()) != (*i)->getPosition() )
						return true;
					break;
				default:
					break;
			}
		}
	}
	return false;
}

bool			CollisionsManager::hasObstacle(Map const & map, glm::vec2 v, std::vector<IGameEntity *> const &entityList,  IGameEntity const * entity){
	return (map.hasBloc(v) || collidesWithEntity(v, entity, entityList));
}

void			CollisionsManager::gestionSlipOneDirection(glm::vec2 & pos, IGameEntity const * entity, Map const & map, std::vector<IGameEntity *> &entityList)
{
	if (entity->getDirection().x > 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x + RPLAYER), ceil(pos.y)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(round(pos.x + RPLAYER), ceil(pos.y));
			angle.x -= 0.5;
			angle.y -= 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x -= 0.01;
				pos.y -= 0.01;
			}
		}
		if (hasObstacle(map, glm::vec2(round(pos.x + RPLAYER), floor(pos.y)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(round(pos.x + RPLAYER), floor(pos.y));
			angle.x -= 0.5;
			angle.y += 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x -= 0.01;
				pos.y += 0.01;
			}
		}
	}
	if (entity->getDirection().x < 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x - RPLAYER), ceil(pos.y)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(round(pos.x - RPLAYER), ceil(pos.y));
			angle.x += 0.5;
			angle.y -= 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x += 0.01;
				pos.y -= 0.01;
			}
		}
		if (hasObstacle(map, glm::vec2(round(pos.x - RPLAYER), floor(pos.y)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(round(pos.x - RPLAYER), floor(pos.y));
			angle.x += 0.5;
			angle.y += 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x += 0.01;
				pos.y += 0.01;
			}
		}
	}
	if (entity->getDirection().y > 0)
	{
		if (hasObstacle(map, glm::vec2(ceil(pos.x), round(pos.y + RPLAYER)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(ceil(pos.x), round(pos.y + RPLAYER));
			angle.x -= 0.5;
			angle.y -= 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x -= 0.01;
				pos.y -= 0.01;
			}
		}
		if (hasObstacle(map, glm::vec2(floor(pos.x), round(pos.y + RPLAYER)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(floor(pos.x), round(pos.y + RPLAYER));
			angle.x += 0.5;
			angle.y -= 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x += 0.01;
				pos.y -= 0.01;
			}
		}
	}
	if (entity->getDirection().y < 0)
	{
		if (hasObstacle(map, glm::vec2(ceil(pos.x), round(pos.y - RPLAYER)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(ceil(pos.x), round(pos.y - RPLAYER));
			angle.x -= 0.5;
			angle.y += 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x -= 0.01;
				pos.y += 0.01;
			}
		}
		if (hasObstacle(map, glm::vec2(floor(pos.x), round(pos.y - RPLAYER)), entityList, entity))
		{
			glm::vec2	angle = glm::vec2(floor(pos.x), round(pos.y - RPLAYER));
			angle.x += 0.5;
			angle.y += 0.5;
			while (distance(pos, angle) < RPLAYER)
			{
				pos.x += 0.01;
				pos.y += 0.01;
			}
		}
	}
}

void			CollisionsManager::gestionSlipBidirection(glm::vec2 &, IGameEntity const *, Map const &)
{

}
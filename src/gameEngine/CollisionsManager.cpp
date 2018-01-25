
#include "CollisionsManager.hpp"

CollisionsManager::CollisionsManager() :
_last_frame_time_point(std::chrono::steady_clock::now()){
}

CollisionsManager::~CollisionsManager() {}

void			CollisionsManager::moves(Map const & map, std::vector<IGameEntity *> &entityList)
{
    _time_since_last_frame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _last_frame_time_point);
	for (std::vector<IGameEntity *>::iterator i = entityList.begin(); i != entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:
				if ((*i)->getState() == State::DYING)
				{
					static_cast<Player*>(*i)->setFrameBeforeDelete(static_cast<Player*>(*i)->getFrameBeforeDelete() + 1);
					if (static_cast<Player*>(*i)->getFrameBeforeDelete() >= 150)
					{
						entityList.erase(i);
						i--;
						continue;
					}
				}
				compute_player(static_cast<Player *>(*i));
				computeEntityMovement(map, entityList, (*i));
				break;
			case Type::ENEMY:
				computeEntityMovement(map, entityList, (*i));
				break;
			default:
				break;
		}
	}
	_last_frame_time_point = std::chrono::steady_clock::now();
}

void		CollisionsManager::computeEntityMovement(Map const & map, std::vector<IGameEntity *> &entityList, IGameEntity *entity){
	if (entity->getState() == State::MOVING){
		glm::vec2	newPos = entity->getPosition() + (entity->getDirection() * entity->getSpeed() * static_cast<float>(_time_since_last_frame.count()) * 0.001f);
		if (entity->getType() == Type::ENEMY)
			newPos = entity->getPosition() + (entity->getDirection() * entity->getSpeed() * static_cast<float>(_time_since_last_frame.count()) * 0.05f);
		
		gestionBorderMap(newPos, map);
		
		if (!gestionNoSlipMove(newPos, entity, map, entityList))
		{
			if (entity->getDirection().x == 0 || entity->getDirection().y == 0)
				gestionSlipOneDirection(newPos, entity, map, entityList);
			else
				gestionSlipBidirection(newPos, entity, map);
		}

		// gestion border

		// set the new position
		//entity->setDirection(normalize(newPos - entity->getPosition()));

		if (entity->getType() == Type::ENEMY){
			if (newPos == entity->getPosition()){
				SEventManager::getInstance().raise(Event::ENEMY_COLLIDES, entity);
			}
			SEventManager::getInstance().raise(Event::ENEMY_MOVE, entity);
		}
		entity->setPosition(newPos);
		if (entity->getType() == Type::PLAYER){
			SEventManager::getInstance().raise(Event::PLAYER_MOVE, entity);
		}
	}
}

void	CollisionsManager::compute_player(Player *p){
	if (p->getState() == State::DYING || p->getState() == State::WINNING)
		return;
	glm::vec2	v = p->getNewDirection();
	if (!std::isnan(v.x) && !std::isnan(v.y)){ // glm::isnan wouldn't compile for some reason...
		p->setState(State::MOVING);
		p->setDirection(v);
		p->setSpeed(PLAYER_BASE_SPEED * p->getSpeedMult());
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
	float rayon = (entity->getType() == Type::PLAYER) ? RPLAYER : 0.5f;

	if (entity->getDirection().x > 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x + rayon), round(pos.y)), entityList, entity))
		{
			isBlocked = 1;
			pos.x = round(pos.x + rayon) - rayon - 0.5;
		}
	}
	if (entity->getDirection().x < 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x - rayon), round(pos.y)), entityList, entity))
		{
			isBlocked = 1;
			pos.x = round(pos.x - rayon) + rayon + 0.5;
		}
	}
	if (entity->getDirection().y > 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x), round(pos.y + rayon)), entityList, entity))
		{
			isBlocked = 1;
			pos.y = round(pos.y + rayon) - rayon - 0.5;
		}
	}
	if (entity->getDirection().y < 0)
	{
		if (hasObstacle(map, glm::vec2(round(pos.x), round(pos.y - rayon)), entityList, entity))
		{
			isBlocked = 1;
			pos.y = round(pos.y - rayon) + rayon + 0.5;
		}
	}

	return isBlocked;
}

bool			CollisionsManager::collidesWithEntity(glm::vec2 &v, IGameEntity const * entity, std::vector<IGameEntity *> const &entityList){
	if (entityList.empty())
		return false;
	for (auto i = entityList.begin(); i != entityList.end(); i++){
		if ((*i) != entity){
			switch((*i)->getType()){
				case Type::BOMB:
															//	this is so a player already on the same spot as a bomb doesn't produce a collision
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

CollisionsManager::CollisionsManager(CollisionsManager const &c) :
_last_frame_time_point(c._last_frame_time_point){
}
CollisionsManager   &CollisionsManager::operator=(CollisionsManager const &rhs){
	_last_frame_time_point = rhs._last_frame_time_point;
	return *this;
}
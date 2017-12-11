
# include "AI.hpp"
# include "Bomb.hpp"
# include <glm/ext.hpp>


class Spot {
    public:
        bool                _free;
        bool                _safe;
        int                 _dist;
        
        Spot() : _free(false) {};
        Spot(bool f, bool s) : _free(f), _safe(s), _dist(-1) {};
        ~Spot() {};
};

AI::AI(Player *player) : _player(player), _debug_cubes(new std::vector<glm::vec2>()), _went_far(false){
    SEventManager & em = SEventManager::getInstance();
    em.raise(Event::AIPTR, _debug_cubes);
}  

AI::~AI() {

}





/////////////////////////           UTILS           //////////////////////


Event::Enum     AI::endEvents(Event::Enum e){
    switch (e){
        case Event::PLAYER_LEFT:    return Event::END_PLAYER_LEFT;
        case Event::PLAYER_RIGHT:   return Event::END_PLAYER_RIGHT;
        case Event::PLAYER_UP:      return Event::END_PLAYER_UP;
        case Event::PLAYER_DOWN:    return Event::END_PLAYER_DOWN;
        default:                     return Event::END_PLAYER_LEFT;
    }
}

glm::vec2     AI::dirToVec(Event::Enum e){
    switch (e){
        case Event::PLAYER_LEFT:    return glm::vec2(-1, 0) ;
        case Event::PLAYER_RIGHT:   return glm::vec2(1, 0) ;
        case Event::PLAYER_UP:      return glm::vec2(0, 1) ;
        case Event::PLAYER_DOWN:    return glm::vec2(0, -1);
        default:                     return glm::vec2(0, 0);
    }
}

Event::Enum    AI::vecToDir(glm::vec2 v){
    if (std::abs(v.x) < 0.1f && std::abs(v.y) < 0.1f)
        return Event::PLAYER_MOVE;
    if (std::abs(v.x) > std::abs(v.y)){
        return (v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    else {
        return (v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
}


std::vector<Event::Enum>  AI::vecToDirs(glm::vec2 v){
    std::vector<Event::Enum> ret;
    if (std::abs(v.x) > 0.05f){
        ret.push_back(v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    if (std::abs(v.y) > 0.05f) {
        ret.push_back(v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
    return ret;
}











bool        AI::would_be_blocked_by_bomb(void){
    Bomb    *potential_bomb = new Bomb(glm::round(_player->getPosition()), _player);
    glm::ivec2  tmp_pos = glm::ivec2(glm::round(_player->getPosition()));
    updateMapSafety(potential_bomb);
    delete potential_bomb;
    bool ret = aimClosestSafeSpace(&tmp_pos);
    return !ret;


}


bool      AI::can_place_bomb(void){
    return (_player->getBombCount() < 1);
}




bool    AI::aimClosestSafeSpace(glm::ivec2 *obj){
    bool ret = false;
    updateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && i.second._safe && (i.second._dist < _map[*obj]._dist || _map[*obj]._dist == -1 || !_map[*obj]._safe)){
            *obj = i.first;
            ret = true;
        }
    }
    return ret;
}

bool    AI::aimFarthestSafeSpace(glm::ivec2 *obj){
    bool ret = false;
    if (!_went_far)
        return ret;
    _went_far = false;
    updateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && i.second._safe && (i.second._dist > _map[*obj]._dist || _map[*obj]._dist == -1 || !_map[*obj]._safe)){
            *obj = i.first;
            ret = true;
        }
    }
    return ret;
}


/////////////////       INTERNAL MAP        ///////////////////////////////////////


void    AI::updateMapDistRec(glm::ivec2 pos, int rec){
    auto it = _map.find(pos);
    if (it != _map.end()){
        if (it->second._free && (it->second._dist == -1 || rec < it->second._dist)){
            it->second._dist = rec;
            updateMapDistRec(glm::ivec2(pos.x + 1, pos.y), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x, pos.y + 1), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x, pos.y - 1), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x - 1, pos.y), rec + 1);
        }
    }
}

void    AI::markBombRangeAsUnsafe(glm::ivec2 pos, glm::ivec2 dir, int range){
    if (_map[pos]._free == false)
        return;
    _map[pos]._safe = false;
    if (range > 0){
        markBombRangeAsUnsafe(pos + dir, dir, range - 1);
    }
}

void    AI::updateMapSafety(IGameEntity *entity){
    glm::ivec2 pos(entity->getPosition());
    if (entity->getType() == Type::BOMB){
        _map[pos]._safe = false;
        markBombRangeAsUnsafe(pos, glm::ivec2(-1, 0), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos, glm::ivec2(1, 0), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos, glm::ivec2(0, -1), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos, glm::ivec2(0, 1), static_cast<Bomb *>(entity)->getFlameNb());
    }
    else if (entity->getType() == Type::FLAME){
         _map[pos]._safe = false;
         _map[pos]._free = false;
    }
}

void    AI::updateMap(Map const & map, std::vector<IGameEntity *> & entities){
    glm::vec2 mapSize = map.getSize();
    
    _map.clear();
    for(unsigned int i = 0; i < mapSize.x; i++) {
        for(unsigned int j = 0; j < mapSize.y; j++) {
            _map.insert(std::pair<glm::vec2, Spot>(glm::vec2(i, j), Spot(!map.hasBloc(glm::vec2(i, j)), true)));
        }
    }
    for (auto i : entities){
        updateMapSafety(i);
    }
}





/////////////////////////////       RUN         ////////////////////////////////////////



void    AI::runToObjective(){
    std::vector<Event::Enum>     min_dir;
    updateMapDistRec(_player->getPosition(), 0);
    int             min_dist = _map[_objective]._dist;

    Event::Enum dirs[4] = {Event::PLAYER_LEFT, Event::PLAYER_RIGHT, Event::PLAYER_UP, Event::PLAYER_DOWN};

    for (auto it : dirs){
        updateMapDistRec(_player->getPosition() + AI::dirToVec(it), 0);
        if (min_dist > _map[_objective]._dist && _map[_player->getPosition() + AI::dirToVec(it)]._free){
            min_dir.clear();
            min_dir.push_back(it);
            // std::cout << glm::to_string(glm::round(_player->getPosition())) << " + " << glm::to_string(AI::dirToVec(it)) <<" => " << glm::to_string(glm::ivec2(_player->getPosition() + AI::dirToVec(it))) << " XX " << glm::to_string(_objective) << std::endl;
            min_dist = _map[_objective]._dist;
        }
    }

    if (min_dir.size() == 0 || glm::length(glm::vec2(_objective) - _player->getPosition()) < 1.30f){
        min_dir = AI::vecToDirs(glm::vec2(_objective) - _player->getPosition());
        if (min_dir.size() == 0)
            _went_far = true;
    }

    for (auto i : _last_dir) {
        if (std::find(min_dir.begin(), min_dir.end(), i) == min_dir.end())
        SEventManager::getInstance().raise(AI::endEvents(i), _player);
    }
    _last_dir.erase(std::remove_if(_last_dir.begin(), _last_dir.end(), [&min_dir](Event::Enum e) {
                return (std::find(min_dir.begin(), min_dir.end(), e) == min_dir.end());
            }), _last_dir.end());
    for (auto i : min_dir){
        if (_last_dir.size() == 0 || std::find(_last_dir.begin(), _last_dir.end(), i) == _last_dir.end()){
            _last_dir.push_back(i);
        }
        SEventManager::getInstance().raise(i, _player);
    }
}













//////////////////////////      COMPUTE         /////////////////////////////



void    AI::compute(Map const & map, std::vector<IGameEntity *> & entities) {
    (void)entities;
    updateMap(map, entities);
    if (can_place_bomb()){
        if (would_be_blocked_by_bomb()){
            updateMap(map, entities);
            aimFarthestSafeSpace(&_objective);
        } else {
            updateMap(map, entities);
            SEventManager::getInstance().raise(Event::SPAWN_BOMB, _player);
            aimClosestSafeSpace(&_objective);
        }
    } else {
        aimClosestSafeSpace(&_objective);
    }
    updateDebugCubes(map, entities);
    runToObjective();
    
}


///////////////////////////      DEBUG       //////////////////////////////

bool    AI::shouldAppearInDebug(glm::ivec2 pos){
    return _map[pos]._free && _map[pos]._dist > 0;
}

void    AI::updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) {
    glm::vec2 mapSize = map.getSize();

    (void)entities;
    (void)mapSize;
    _debug_cubes->clear();
    if (_map.size() == 0)
        return;



    for (auto i : _map) {
        if (shouldAppearInDebug(i.first))
            _debug_cubes->push_back(i.first);
    }
    // _debug_cubes->push_back(_objective);
    
}
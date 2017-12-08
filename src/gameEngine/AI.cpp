
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

AI::AI(Player *player) : _player(player), _debug_cubes(new std::vector<glm::vec2>()){
    SEventManager & em = SEventManager::getInstance();
    em.raise(Event::AIPTR, _debug_cubes);
}  

AI::~AI() {

}

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
    std::cout << "a" << std::endl;
    if (std::abs(v.x) > std::abs(v.y)){
        return (v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    else {
        return (v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
}

std::vector<Event::Enum>  AI::vecsToDir(glm::vec2 v){
    std::vector<Event::Enum> ret;
    if (std::abs(v.x) > 0.05f){
        ret.push_back(v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    if (std::abs(v.y) > 0.05f) {
        ret.push_back(v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
    return ret;
}

bool      AI::can_place_bomb(void){
    return (_player->getBombCount() < 1);
}

bool    AI::is_safe(glm::ivec2 pos, std::vector<IGameEntity *> & entities){
    for (auto it : entities){
        if (it->getType() == Type::BOMB && ((it->getPosition().x == pos.x && std::abs(it->getPosition().y - pos.y) <= static_cast<Bomb*>(it)->getFlameNb())
        || (it->getPosition().y == pos.y && std::abs(it->getPosition().x - pos.x) <= static_cast<Bomb*>(it)->getFlameNb())))
            return false;
        if (it->getType() == Type::FLAME && (it->getPosition().x == pos.x && it->getPosition().y == pos.y))
            return false;
    }
    return true;
}

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

void    AI::updateObjective(void){
    updateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && i.second._safe && (i.second._dist < _map[_objective]._dist || _map[_objective]._dist == -1 || !_map[_objective]._safe)){
            _objective = i.first;
        }
    }
}

void    AI::updateMap(Map const & map, std::vector<IGameEntity *> & entities){
    glm::vec2 mapSize = map.getSize();
    
    _map.clear();
    for(unsigned int i = 0; i < mapSize.x; i++) {
        for(unsigned int j = 0; j < mapSize.y; j++) {
            _map.insert(std::pair<glm::vec2, Spot>(glm::vec2(i, j), Spot(!map.hasBloc(glm::vec2(i, j)), is_safe(glm::ivec2(i, j), entities))));
        }
    }
}

void    AI::runToObjective(){
    std::vector<Event::Enum>     min_dir;
    updateMapDistRec(_player->getPosition(), 0);
    int             min_dist = _map[_objective]._dist;

    Event::Enum dirs[4] = {Event::PLAYER_LEFT, Event::PLAYER_RIGHT, Event::PLAYER_UP, Event::PLAYER_DOWN};

    for (auto it : dirs){
        updateMapDistRec(_player->getPosition() + AI::dirToVec(it), 0);
        if (min_dist > _map[_objective]._dist && _map[glm::round(_player->getPosition()) + AI::dirToVec(it)]._free){
            min_dir.clear();
            min_dir.push_back(it);
            std::cout << glm::to_string(glm::round(_player->getPosition())) << " + " << glm::to_string(AI::dirToVec(it)) <<" => " << glm::to_string(glm::ivec2(_player->getPosition() + AI::dirToVec(it))) << " XX " << glm::to_string(_objective) << std::endl;
            min_dist = _map[_objective]._dist;
        }
    }

    if (min_dir.size() == 0){
        min_dir = AI::vecsToDir(glm::vec2(_objective) - _player->getPosition());
    }
    // std::cout << glm::to_string(_player->getPosition()) << " X " << glm::to_string(_objective) << std::endl;

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

void    AI::run_to_safety(Map const & map, std::vector<IGameEntity *> & entities){
    (void)map;
    (void)entities;
    // if (is_safe(glm::round(_player->getPosition()), entities)){
    //     SEventManager::getInstance().raise(AI::endEvents(_last_dir), _player);
    //     return;
    // }
    updateObjective();
    runToObjective();
}

void    AI::compute(Map const & map, std::vector<IGameEntity *> & entities) {
    (void)entities;
    updateMap(map, entities);
    run_to_safety(map, entities);
    updateDebugCubes(map, entities);
    if (can_place_bomb()){
        SEventManager::getInstance().raise(Event::SPAWN_BOMB, _player);
    }
    // run_to_safety(map, entities);
    
}

bool    AI::shouldAppearInDebug(glm::ivec2 pos){
    // std::cout << (_map[pos]._free && _map[pos]._safe && _map[pos]._dist > 3) << std::endl;
    return _map[pos]._free && _map[pos]._safe && _map[pos]._dist > 0;
    // return pos == _objective;
}

void    AI::updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) {
    glm::vec2 mapSize = map.getSize();

    (void)entities;
    (void)mapSize;
    _debug_cubes->clear();
    if (_map.size() == 0)
        return;



    // for (auto i : _map) {
    //     if (shouldAppearInDebug(i.first))
    //         _debug_cubes->push_back(i.first);
    // }
    _debug_cubes->push_back(_objective);
    
}
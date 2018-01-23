#ifndef PLAYERSMANAGER_HPP
# define PLAYERSMANAGER_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Map.hpp"
# include "AI.hpp"
# include "CollisionsManager.hpp"
# include "SEventManager.hpp"
# include "Player.hpp"


// Links Window Events to actual player movement
// Handle IA Players movement
class PlayerManager {
	public:
		PlayerManager();
		~PlayerManager();

        void                compute(Map const & map, std::vector<IGameEntity *> &entityList);
        void                setHumanPlayer(Player *);
        void                addPlayer(Player *);
        void                Register(void);
        void                unRegister(void);

private:

    Player                  *_human_player;
    std::vector<AI *>       _AIs;
    bool					_spawned_bomb;
    bool					_active;

    void                    ai(Player *p, Map const & map, std::vector<IGameEntity *> &entityList);
    void                    newDirLeft(void *p);
    void                    newDirRight(void *p);
    void                    newDirUp(void *p);
    void                    newDirDown(void *p);
    void                    endNewDirLeft(void *p);
    void                    endNewDirRight(void *p);
    void                    endNewDirUp(void *p);
    void                    endNewDirDown(void *p);
    void                    humanNewDirLeft(void *);
    void                    humanNewDirRight(void *);
    void                    humanNewDirUp(void *);
    void                    humanNewDirDown(void *);
    void                    humanEndNewDirLeft(void *);
    void                    humanEndNewDirRight(void *);
    void                    humanEndNewDirUp(void *);
    void                    humanEndNewDirDown(void *);
    void                    playerDies(void *p);


    void                    humanSpawnBomb(void *);
    void                    humanEndSpawnBomb(void *);

};

#endif
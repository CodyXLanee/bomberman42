/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enemy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 16:19:24 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 10:43:52 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_HPP
# define ENEMY_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"

namespace EnemyType {
    enum Enum {BALOON};
};

class Enemy : public AGameEntity {
public:
    Enemy(const glm::vec2 & pos, EnemyType::Enum type);
    ~Enemy();


    glm::vec2				getGraphicalDirection() const;
    void					setGraphicalDirection(glm::vec2 dir);

    int                     getCounterDying() const;
    void                    setCounterDying(int counter);

    EnemyType::Enum         getEnemyType(void);
    void                    player_move_callback(void *player);
    void	                spawn_flame_callback(void *flame);


    Enemy(void);
    Enemy(Enemy const &);
    Enemy   &operator=(Enemy const &rhs);
private:
    EnemyType::Enum _type;
	glm::vec2		_graphicalDirection;
    int             _counter_dying;
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enemy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 16:19:24 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/14 22:22:36 by egaborea         ###   ########.fr       */
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


    EnemyType::Enum         getEnemyType(void);
    void                    player_move_callback(void *player);
    void	                spawn_flame_callback(void *flame);

private:
    EnemyType::Enum _type;
};

#endif
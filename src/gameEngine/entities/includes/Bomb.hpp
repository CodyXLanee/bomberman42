/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bomb.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 10:27:23 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/22 15:42:05 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMB_HPP
# define BOMB_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"
# include "Player.hpp"
# include <chrono>

class Player;

class Bomb : public AGameEntity {
public:
    Bomb(const glm::vec2 & pos, Player *p);
    Bomb(const glm::vec2 & pos);
    ~Bomb();
    virtual void    update(void);

    int             getFlameNb() const;

    std::chrono::milliseconds const                                &get_ms_before_explode(void) const;
    std::chrono::milliseconds const                                get_ms_remaining_before_explode(void) const;
    std::chrono::time_point<std::chrono::steady_clock> const       &get_creation_time(void) const;
    const Player                                                   *getPlayer(void) const;

private:
    void                                                    explode_if_touched(void *p);
    void                                                    explode(void);
    std::chrono::milliseconds                               ms_before_explode;
    std::chrono::time_point<std::chrono::steady_clock>      creation_time;
    Player                                                  *player;
};

#endif
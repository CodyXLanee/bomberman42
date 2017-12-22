/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:18:42 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/22 16:17:02 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAME_HPP
# define FLAME_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"
# include <chrono>

class Flame : public AGameEntity {
public:
    Flame(const glm::vec2 & pos, bool b);
    ~Flame();
    virtual void    update(void);

    std::chrono::milliseconds const                                &get_ms_before_explode(void) const;
    std::chrono::time_point<std::chrono::steady_clock> const       &get_creation_time(void) const;

    bool                                                            is_centered(void) const;

private:
    void            player_move_callback(void *player);
    void            enemy_move_callback(void *enemy);

    std::chrono::milliseconds                               ms_before_explode;
    std::chrono::time_point<std::chrono::steady_clock>      creation_time;
    bool                                                    _center;
};

#endif
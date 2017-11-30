/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:18:42 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/30 16:02:00 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAME_HPP
# define FLAME_HPP

#include "AGameEntity.hpp"
#include <chrono>

class Flame : public AGameEntity {
public:
    Flame(const glm::vec2 & pos);
    ~Flame();
    virtual void    update(void);

    std::chrono::milliseconds const                                &get_ms_before_explode(void) const;
    std::chrono::time_point<std::chrono::steady_clock> const       &get_creation_time(void) const;

private:
    std::chrono::milliseconds                               ms_before_explode;
    std::chrono::time_point<std::chrono::steady_clock>      creation_time;
};

#endif
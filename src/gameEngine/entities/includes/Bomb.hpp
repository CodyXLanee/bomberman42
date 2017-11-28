/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bomb.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 10:27:23 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/28 15:02:19 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMB_HPP
# define BOMB_HPP

#include "AGameEntity.hpp"
#include <chrono>

class Bomb : public AGameEntity {
public:
    Bomb(const glm::vec2 & pos);
    ~Bomb();
    virtual void    update(void);
private:
    std::chrono::milliseconds                               ms_before_explode;
    std::chrono::time_point<std::chrono::steady_clock>      creation_time;
};

#endif
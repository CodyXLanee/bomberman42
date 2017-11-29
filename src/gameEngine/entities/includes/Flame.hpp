/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:18:42 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/29 12:42:07 by egaborea         ###   ########.fr       */
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
private:
    std::chrono::milliseconds                               ms_before_explode;
    std::chrono::time_point<std::chrono::steady_clock>      creation_time;
};

#endif
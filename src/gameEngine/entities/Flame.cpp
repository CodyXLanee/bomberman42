/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:24:42 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/30 16:01:52 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Flame.hpp"

Flame::Flame(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::FLAME){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(1000);
}

Flame::~Flame(){
}

void    Flame::update(void){
    std::chrono::milliseconds   time_since_creation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creation_time);
    if (time_since_creation > ms_before_explode) {
        setState(State::DYING);
    }
}

std::chrono::milliseconds const                                &Flame::get_ms_before_explode(void) const {
    return ms_before_explode;
}
std::chrono::time_point<std::chrono::steady_clock> const       &Flame::get_creation_time(void) const {
    return creation_time;
}
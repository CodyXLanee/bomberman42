/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:24:42 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/29 12:39:15 by egaborea         ###   ########.fr       */
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
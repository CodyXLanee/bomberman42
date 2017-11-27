/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:45:55 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 16:40:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

#include "AGameEntity.hpp"

class Player : public AGameEntity {
public:
    Player(const glm::vec2 & pos);
};

#endif
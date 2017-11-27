/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndestructibleBloc.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 16:06:28 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 16:37:19 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IndestructibleBloc.hpp"

IndestructibleBloc::IndestructibleBloc(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::INDESTRUCTIBLE_BLOC){
}
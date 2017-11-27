/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DestructibleBloc.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 16:04:10 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 16:36:21 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DestructibleBloc.hpp"

DestructibleBloc::DestructibleBloc(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::DESTRUCTIBLE_BLOC) {
}
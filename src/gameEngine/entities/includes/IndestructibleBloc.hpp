/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndestructibleBloc.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 16:05:50 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 16:40:29 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDESTRUCTIBLEBLOC_HPP
# define INDESTRUCTIBLEBLOC_HPP

#include "AGameEntity.hpp"

class IndestructibleBloc : public AGameEntity {
public:
    IndestructibleBloc(const glm::vec2 & pos);
};

#endif
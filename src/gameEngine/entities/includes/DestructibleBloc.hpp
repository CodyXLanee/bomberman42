/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DestructibleBloc.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 16:02:14 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 16:40:20 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DESTRUCTIBLEBLOC_HPP
# define DESTRUCTIBLEBLOC_HPP

#include "AGameEntity.hpp"

class DestructibleBloc : public AGameEntity {
public:
    DestructibleBloc(const glm::vec2 & pos);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:47:02 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 15:54:00 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IGAMEENTITY_HPP
# define IGAMEENTITY_HPP

# include <glm/gtc/matrix_transform.hpp>

namespace State{
	enum Enum { STAND };
}

namespace Type{
	enum Enum { PLAYER, BOMB};
}

class IGameEntity {
	public:
		virtual ~IGameEntity() {};
		virtual glm::vec2	getPositon() const = 0;
		virtual State::Enum	getState() const = 0;
		virtual float		getSpeed() const = 0;
		virtual Type::Enum	getType() const = 0;
};

#endif
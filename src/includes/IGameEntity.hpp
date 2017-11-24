/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:47:02 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 17:29:15 by lfourque         ###   ########.fr       */
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
		virtual glm::vec2	getPosition() const = 0;
		virtual State::Enum	getState() const = 0;
		virtual float		getSpeed() const = 0;
		virtual Type::Enum	getType() const = 0;
};

#endif
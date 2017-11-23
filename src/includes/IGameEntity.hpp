/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:47:02 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 17:04:41 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IGAMEENTITY_HPP
# define IGAMEENTITY_HPP

# include <glm/gtc/matrix_transform.hpp>

namespace State{
	enum Enum { STAND };
}

class IGameEntity {
	public:
		virtual ~IGameEntity() = 0;
		virtual glm::vec2	getPositon() = 0;
		virtual State::Enum	getState() = 0;
		virtual float		getSpeed() = 0;
};

#endif
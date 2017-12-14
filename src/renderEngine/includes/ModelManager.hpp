/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:17:58 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/14 17:59:20 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODELMANAGER_HPP
# define MODELMANAGER_HPP

# include "Model.hpp"

class ModelManager {
	public:
		enum model { PLAYER, GROUND, WALL, BRICK, BOMB, FLAME, AIDEBUG, FLAME_UP, BOMB_UP, SPEED_UP, SCENERY, BALOON };



		ModelManager();
		~ModelManager();

		Model	&getModel(model m) const;
		
	private:

		std::string	directory;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Model		*bombModel;
		Model		*flameModel;
		Model		*aiDebug;
		Model		*flameUpModel;
		Model		*bombUpModel;
		Model		*speedUpModel;
		Model		*baloonModel;
		Model		*sceneryModel;
};

#endif
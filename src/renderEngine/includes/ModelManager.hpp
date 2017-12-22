/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:17:58 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/21 13:52:11 by tpierron         ###   ########.fr       */
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
		Model	&getPlayerModel(unsigned int n) const;
		
	private:

		std::string	directory;
		
		// Model		*playerModel;
		Model		*playerModel[4];
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
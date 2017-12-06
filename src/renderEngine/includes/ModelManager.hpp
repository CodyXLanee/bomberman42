/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:17:58 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 15:22:33 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODELMANAGER_HPP
# define MODELMANAGER_HPP

# include "Model.hpp"

class ModelManager {
	public:
		enum model { PLAYER, GROUND, WALL, BRICK, BOMB, FLAME };

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
};

#endif
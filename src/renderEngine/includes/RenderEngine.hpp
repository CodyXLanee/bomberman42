/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:25:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 10:33:36 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
// # include "Camera.hpp"
# include "Map.hpp"
# include <vector>

class RenderEngine {
	
	public:
		RenderEngine();
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity const *> const & entities);

	private:
		Shader	*shader;
		Model	*model;
		// Camera  camera;

		void	renderMap() const;
		void	renderGround() const;
		void	setCamera();
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:25:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 03:49:39 by egaborea         ###   ########.fr       */
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

# include <OpenGL/gl3.h>
# include <SDL.h>

class RenderEngine {
	
	public:
		RenderEngine(SDL_Window	*win);
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity *> const & entities, glm::mat4 const & cameraMatrix);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		// Camera  camera;

		void	renderMap() const;
		void	renderPlayer(IGameEntity const *) const;
		void	renderGround() const;
		void	renderWall() const;
		void	setCamera(glm::mat4 const &);
};

#endif
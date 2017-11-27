/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:25:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 17:24:31 by tpierron         ###   ########.fr       */
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
		
		void	render(Map const & map, std::vector<IGameEntity const *> const & entities, glm::mat4 const & cameraMatrix);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Shader		*textureShader;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		// Camera  camera;

		void	renderMap() const;
		void	renderWall() const;
		void	renderBrick() const;
		void	renderPlayer() const;
		void	renderGround() const;
		void	setCamera(glm::mat4 const &);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:25:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 14:42:52 by tpierron         ###   ########.fr       */
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
		
		void	render(Map const & map, std::vector<IGameEntity const *> const & entities);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		// Camera  camera;

		void	renderMap() const;
		void	renderPlayer() const;
		void	renderGround() const;
		void	renderWall() const;
		void	setCamera();
};

#endif
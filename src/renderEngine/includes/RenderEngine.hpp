/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 11:44:44 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Map.hpp"
# include <vector>

# include <OpenGL/gl3.h>
# include <SDL.h>

class RenderEngine {
	
	public:
		RenderEngine(SDL_Window	*win, Camera &);
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity *> const & entities);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Shader		*textureShader;
		Shader		*shadowShader;
		Shader		*debugDepthQuad;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Camera  	&camera;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int quadVAO;
		unsigned int quadVBO;

		void	getShadowMap();
		void	renderScene(Shader *shader) const;
		void	renderWall(Shader *shader) const;
		void	renderBrick(Shader *shader) const;
		void	renderGround(Shader *shader) const;
		void	renderPlayer(IGameEntity const *) const;
		void	setCamera(glm::mat4 const &);

		void	createShadowBuffer();

		void	renderShadowMap();
};

#endif
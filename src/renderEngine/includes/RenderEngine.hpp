/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/01 16:31:51 by tpierron         ###   ########.fr       */
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
		Shader		*directionalShadowShader;
		Shader		*pointShadowShader;
		Shader		*debugDepthQuad;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Camera  	&camera;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int depthCubemap;
		unsigned int quadVAO;
		unsigned int quadVBO;

		void	renderScene(Shader *shader, Map const & map, std::vector<IGameEntity *> &entities) const;
		void	renderWall(Shader *shader, const std::vector<IndestructibleBloc> &b, Map const & map) const;
		void	renderBrick(Shader *shader, const std::vector<DestructibleBloc> &blocs, Map const & map) const;
		void	renderGround(Shader *shader, Map const & map) const;
		void	renderPlayer(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBombs(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderFlames(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		
		void	getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		void	getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		// void	setCamera(glm::mat4 const &);
		void	setCamera(glm::mat4 const &, Shader *shader) const;

		void	createShadowBuffer();
		void	createDepthCubemap();

		void	renderShadowMap();
};

#endif
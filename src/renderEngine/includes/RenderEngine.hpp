/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/04 14:20:58 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Light.hpp"
# include "ParticleSystem.hpp"
# include "NuklearGUI.hpp"
# include "Map.hpp"
# include <vector>

# include <OpenGL/gl3.h>
# include <SDL.h>

class RenderEngine {
	
	public:
		RenderEngine(SDL_Window	*win, Camera &);
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity *> & entities);
		void	renderGUI(std::vector<Action::Enum> &);
		NuklearGUI &	getGUI();
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*mainShader;
		Shader		*flamesShader;
		Shader		*directionalShadowShader;
		Shader		*pointShadowShader;
		Shader		*particlesShader;
		Shader		*debugDepthQuad;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Model		*bombModel;
		Model		*flameModel;
		Camera  	&camera;
		Light		*light;
		NuklearGUI	gui;

		ParticleSystem *particles;

		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int depthCubemap;
		unsigned int quadVAO;
		unsigned int quadVBO;

		void	getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		void	getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		void	renderScene(Shader *shader, Map const & map, std::vector<IGameEntity *> &entities) const;
		void	renderWall(Shader *shader, const std::vector<IndestructibleBloc> &b, Map const & map) const;
		void	renderBrick(Shader *shader, const std::vector<DestructibleBloc> &blocs, Map const & map) const;
		void	renderGround(Shader *shader, Map const & map) const;
		void	renderPlayer(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBombs(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderFlames(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderParticles();
		void	setCamera(glm::mat4 const &, Shader *shader) const;

		void	createShadowBuffer();
		void	createDepthCubemap();

		void	renderShadowMap();
};

#endif
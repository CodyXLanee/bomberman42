/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 12:36:20 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "ModelManager.hpp"
# include "Camera.hpp"
# include "Light.hpp"
# include "ParticleSystem.hpp"
# include "WeatherSystem.hpp"
# include "NuklearGUI.hpp"
# include "Map.hpp"
# include "Player.hpp"
# include "Flame.hpp"
# include "Bomb.hpp"
# include <cmath>
# include <glm/ext.hpp>
# include <glm/gtx/vector_angle.hpp>
# include <vector>

# include <OpenGL/gl3.h>
# include <SDL.h>

class RenderEngine {
	
	public:
		RenderEngine(SDL_Window	*win, Camera &);
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity *> & entities);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*mainShader;
		Shader		*flamesShader;
		Shader		*directionalShadowShader;
		Shader		*pointShadowShader;
		Shader		*particlesShader;
		Shader		*debugDepthQuad;
		
		// Model		*playerModel;
		// Model		*groundModel;
		// Model		*wallModel;
		// Model		*brickModel;
		// Model		*bombModel;
		// Model		*flameModel;
		ModelManager	modelManager;
		Camera  	&camera;
		Light		*light;

		WeatherSystem	*meteo;

		std::vector<ParticleSystem *> particles;
		std::vector<IGameEntity *> entitiesRecorded;

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
		void	renderParticles() const;

		// void	recordNewEntities(std::vector<IGameEntity *> & entities);

		void	setCamera(glm::mat4 const &, Shader *shader) const;

		void	createShadowBuffer();
		void	createDepthCubemap();
		void	getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		void	getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities);

		void	renderShadowMap();
};

#endif
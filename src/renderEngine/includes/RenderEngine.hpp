/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/08 09:27:55 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "ModelManager.hpp"
# include "ShaderManager.hpp"
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
		
		ModelManager	modelManager;
		ShaderManager	shaderManager;
		Camera  	&camera;
		Light		*light;

		WeatherSystem	*meteo;

		std::vector<ParticleSystem *>	particles;
		std::vector<IGameEntity *>		entitiesRecorded;
		std::vector<glm::vec3>			fireLights;

		int w;
		int h;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int depthCubemap;

		void	renderScene(Shader &shader, Map const & map, std::vector<IGameEntity *> &entities) const;
		void	renderWall(Shader &shader, const std::vector<IndestructibleBloc> &b, Map const & map) const;
		void	renderBrick(Shader &shader, const std::vector<DestructibleBloc> &blocs, Map const & map) const;
		void	renderGround(Shader &shader, Map const & map) const;
		void	renderPlayer(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBombs(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderFlames(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderParticles() const;

		void	setFireLights(std::vector<IGameEntity *> const & entities);
		// void	recordNewEntities(std::vector<IGameEntity *> & entities);
		void	shadowPass(Map const & map, std::vector<IGameEntity *> &entities) const;
		void	normalPass(Map const & map, std::vector<IGameEntity *> &entities) const;
		void	blendedPass(std::vector<IGameEntity *> &entities) const;

		void	createShadowBuffer();
		void	createDepthCubemap();
		void	getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) const;
		void	getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) const;

		std::vector<glm::vec2>	*aiDebugInfo;
		void	setAiDebugPointer(void* ptr);
		void	renderAiDebug(Shader &shader) const;
};

#endif
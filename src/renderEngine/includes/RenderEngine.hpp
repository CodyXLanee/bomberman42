/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/03 14:54:01 by egaborea         ###   ########.fr       */
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
# include "FireParticles.hpp"
# include "BombParticles.hpp"
# include "WeatherSystem.hpp"
# include "NuklearGUI.hpp"
# include "Map.hpp"
# include "Player.hpp"
# include "Enemy.hpp"
# include "Flame.hpp"
# include "Bomb.hpp"
# include "Bonus.hpp"
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

		RenderEngine(RenderEngine const & src);
		RenderEngine&	operator=(RenderEngine const & rhs);
		
		void	render(Map const & map, std::vector<IGameEntity *> & entities);

		SDL_Window*		getWindow() const;
		Camera&			getCamera() const;
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		
		ModelManager	modelManager;
		ShaderManager	shaderManager;
		Camera  		&camera;

		WeatherSystem	*meteo;

		FireParticles			fireParticles;
		BombParticles			bombParticles;

		std::vector<glm::vec3>			fireLights;
		void	addBombParticles(void *);
		void	removeBombParticles(void *);
		void	setFireParticles(void *);

		int w;
		int h;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int depthCubemap;

		void	renderScene(Shader &shader, Map const & map, std::vector<IGameEntity *> &entities) const;
		void	renderWall(Shader &shader, const std::vector<IndestructibleBloc> &b, Map const & map) const;
		void	renderBrick(Shader &shader, const std::vector<DestructibleBloc> &blocs) const;
		void	renderGround(Shader &shader, Map const & map) const;
		void	renderPlayer(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBombs(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBonus(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderFlames(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderEnemies(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderWinSpot(Shader &shader, std::vector<IGameEntity *> const & entities) const;
		void	renderScenery(Shader &shader) const;

		void	setFireLights(std::vector<IGameEntity *> const & entities);
		void	shadowPass(Map const & map, std::vector<IGameEntity *> &entities) const;
		void	normalPass(Map const & map, std::vector<IGameEntity *> &entities) const;
		void	blendedPass(std::vector<IGameEntity *> &entities);

		void	createShadowBuffer();
		void	createDepthCubemap();
		void	getDirectionalShadowMap() const;
		// void	getOmnidirectionalShadowMap() const;

		std::vector<glm::vec2>	*aiDebugInfo;
		void	setAiDebugPointer(void* ptr);
		void	renderAiDebug(Shader &shader) const;
};

#endif
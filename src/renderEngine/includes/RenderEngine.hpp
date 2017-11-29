/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 14:16:30 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "Camera.hpp"
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
		struct nk_context *	getGUIContext() const;
		
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
		Model		*bombModel;
		Camera  	&camera;
		NuklearGUI	gui;

		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int quadVAO;
		unsigned int quadVBO;

		void	getShadowMap(Map const & map, std::vector<IGameEntity *> &entities);
		void	renderScene(Shader *shader, Map const & map, std::vector<IGameEntity *> &entities) const;
		void	renderWall(Shader *shader, const std::vector<IndestructibleBloc> &b) const;
		void	renderBrick(Shader *shader, const std::vector<DestructibleBloc> &blocs) const;
		void	renderGround(Shader *shader) const;
		void	renderPlayer(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	renderBombs(Shader *shader, std::vector<IGameEntity *> const & entities) const;
		void	setCamera(glm::mat4 const &);

		void	createShadowBuffer();

		void	renderShadowMap();
};

#endif
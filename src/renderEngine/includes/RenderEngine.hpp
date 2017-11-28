/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/28 17:13:35 by egaborea         ###   ########.fr       */
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
		
		void	render(Map const & map, std::vector<IGameEntity *> const & entities);
		void	renderGUI(std::vector<Action::Enum> const &);
		struct nk_context *	getGUIContext() const;
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Shader		*textureShader;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Model		*bombModel;
		Camera  	&camera;
		NuklearGUI	gui;
		

		void	renderMap(Map const & map) const;
		void	renderWall(const std::vector<IndestructibleBloc>	&) const;
		void	renderBrick(const std::vector<DestructibleBloc>	&) const;
		void	renderPlayer(IGameEntity const *) const;
		void	renderBombs(std::vector<IGameEntity *> const &);
		void	renderGround() const;
		void	setCamera(glm::mat4 const &);
};

#endif
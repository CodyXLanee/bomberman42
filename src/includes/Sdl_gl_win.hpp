/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 13:18:30 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/18 12:49:56 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_GL_WIN_HPP
# define SDL_GL_WIN_HPP

# include <OpenGL/gl3.h>
# include <SDL.h>
# include <iostream>
# include <vector>

# include "bomberman.hpp"
# include "Shader.hpp"
# include "SEventManager.hpp"
# include "nuklear.h"
# include "nuklear_sdl_gl3.h"

class Sdl_gl_win {
	public:
		Sdl_gl_win(size_t width, size_t height);
		~Sdl_gl_win();

		Sdl_gl_win(Sdl_gl_win const & src);
		Sdl_gl_win& operator=(Sdl_gl_win const & rhs);
		
		void			eventManager(struct nk_context *);
		int				getMouseX() const;
		int				getMouseY() const;
		SDL_Window		*getWin() const;
		std::vector<SDL_DisplayMode> const &	getDisplayModes() const;
		void			initGL() const;
		
		std::map<Event::Enum, SDL_Keycode>	getKeyMap() const;
		void									setKeyMap(std::map<Event::Enum, SDL_Keycode>);
		
	private:
		Sdl_gl_win();
		void			initSDL();
		void			updateScreenFormat(void *);
		void            updateKeyMap(void *k);

		SDL_Event		events;
		SDL_GLContext	ctx;
		SDL_Window		*win;
		std::vector<SDL_DisplayMode>	modes;
		int				mouseX;
		int				mouseY;
		size_t			width;
		size_t			height;
		std::map<Event::Enum, SDL_Keycode>	keyMap;
};

#endif
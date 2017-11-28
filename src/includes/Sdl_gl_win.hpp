/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 13:18:30 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/28 14:55:50 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_GL_WIN_HPP
# define SDL_GL_WIN_HPP

# include <OpenGL/gl3.h>
# include <SDL.h>
# include <iostream>
# include <vector>

# include "bomberman.hpp"
# include "nuklear.h"
# include "nuklear_sdl_gl3.h"

class Sdl_gl_win {
	public:
		Sdl_gl_win();
		~Sdl_gl_win();

		void			eventManager(std::vector<Action::Enum> &, struct nk_context *);
		int				getMouseX() const;
		int				getMouseY() const;
		SDL_Window		*getWin() const;
		void			initGL() const;

	private:
		void			initSDL();

		SDL_Event		events;
		SDL_GLContext	ctx;
		SDL_Window		*win;
		int				mouseX;
		int				mouseY;
};

#endif
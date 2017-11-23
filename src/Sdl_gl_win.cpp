/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:34:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 14:12:11 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sdl_gl_win.hpp"

Sdl_gl_win::Sdl_gl_win() {
	initSDL();
    initGL();
}

Sdl_gl_win::~Sdl_gl_win() {
	SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();

	return;
}

void	Sdl_gl_win::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    win = SDL_CreateWindow("ft_vox",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    ctx = SDL_GL_CreateContext(win);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    if(ctx == 0) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
}

void	Sdl_gl_win::initGL() const {
    glClearColor(0.f, 0.f, 0.f, 0.f);
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void    Sdl_gl_win::eventManager(std::vector<Action::Enum> & actions) {
    SDL_GetRelativeMouseState(&mouseX,&mouseY);
    while (SDL_PollEvent(&events)) {
        if (events.window.event == SDL_WINDOWEVENT_CLOSE ||
            (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE)) {
                actions.clear();
            return actions.push_back(Action::ESCAPE);
        }
    
        if (events.type == SDL_KEYDOWN) {
            Action::Enum a;
            switch(events.key.keysym.sym) {
                case SDLK_a: a = Action::LEFT; break;
                case SDLK_d: a = Action::RIGHT; break;
                case SDLK_w: a = Action::FORWARD; break;
                case SDLK_s: a = Action::BACKWARD; break;
                case SDLK_c: a = Action::DOWN; break;
                case SDLK_SPACE: a = Action::UP; break;
                case SDLK_TAB: a = Action::DEBUG; break;
                case SDLK_LSHIFT: a = Action::TOGGLESPEED; break;
                case SDLK_f: a = Action::ERASE; break;
            }
			if (find(actions.begin(), actions.end(), a) == actions.end())
                actions.push_back(a);
			else if (a == Action::DEBUG || a == Action::TOGGLESPEED)
				actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());
        } 
        if (events.type == SDL_KEYUP) {
            Action::Enum a;
            switch(events.key.keysym.sym) {
                case SDLK_a: a = Action::LEFT; break;
                case SDLK_d: a = Action::RIGHT; break;
                case SDLK_w: a = Action::FORWARD; break;
                case SDLK_s: a = Action::BACKWARD; break;
                case SDLK_c: a = Action::DOWN; break;
                case SDLK_SPACE: a = Action::UP; break;
            }
			actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());
        }
    }
}

SDL_Window      *Sdl_gl_win::getWin() const {
    return win;
}

int             Sdl_gl_win::getMouseX() const {
    return mouseX;
}

int             Sdl_gl_win::getMouseY() const {
    return mouseY;
}

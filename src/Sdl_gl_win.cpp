/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:34:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 22:09:01 by egaborea         ###   ########.fr       */
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
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    win = SDL_CreateWindow("Bomberman",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    ctx = SDL_GL_CreateContext(win);
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    
    if(ctx == 0) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
}

void	Sdl_gl_win::initGL() const {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void    Sdl_gl_win::eventManager(std::vector<Action::Enum> & actions, struct nk_context *nk_ctx) {
    SDL_GetRelativeMouseState(&mouseX,&mouseY);
    nk_input_begin(nk_ctx);
    while (SDL_PollEvent(&events)) {
        if (events.window.event == SDL_WINDOWEVENT_CLOSE ||
            (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE)) {
                actions.clear();
            return actions.push_back(Action::ESCAPE);
        }
        nk_sdl_handle_event(&events);
        if (events.type == SDL_KEYDOWN) {
            Action::Enum a;
            switch(events.key.keysym.sym) {

                case SDLK_a:        a = Action::LEFT; break;
                case SDLK_d:        a = Action::RIGHT; break;
                case SDLK_w:        a = Action::UP; break;
                case SDLK_s:        a = Action::DOWN; break;
                case SDLK_LEFT:     a = Action::CAMERA_LEFT; break;
                case SDLK_RIGHT:    a = Action::CAMERA_RIGHT; break;
                case SDLK_UP:       a = Action::CAMERA_UP; break;
                case SDLK_DOWN:     a = Action::CAMERA_DOWN; break;
                case SDLK_TAB:      a = Action::DEBUG_MODE; break;
                case SDLK_1:        a = Action::FOLLOW_PLAYER; break;
                case SDLK_RETURN:   a = Action::RESET_CAMERA; break;
                case SDLK_LCTRL:    a = Action::MENU; break;
                case SDLK_SPACE:    a = Action::SPAWN_BOMB; break;
            }
			if (find(actions.begin(), actions.end(), a) == actions.end())
                actions.push_back(a);
            else if (a == Action::DEBUG_MODE || a == Action::MENU)
		    	actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());            
        } 
        if (events.type == SDL_KEYUP) {
            Action::Enum a;
            switch(events.key.keysym.sym) {
                case SDLK_a:        a = Action::LEFT; break;
                case SDLK_d:        a = Action::RIGHT; break;
                case SDLK_w:        a = Action::UP; break;
                case SDLK_s:        a = Action::DOWN; break;
                case SDLK_LEFT:     a = Action::CAMERA_LEFT; break;
                case SDLK_RIGHT:    a = Action::CAMERA_RIGHT; break;
                case SDLK_UP:       a = Action::CAMERA_UP; break;
                case SDLK_DOWN:     a = Action::CAMERA_DOWN; break;
                case SDLK_RETURN:   a = Action::RESET_CAMERA; break;
                case SDLK_SPACE:    a = Action::SPAWN_BOMB; break;
            }
			actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());
        }
    }
    nk_input_end(nk_ctx);
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

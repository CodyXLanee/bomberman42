/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:34:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/05 20:25:28 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define NK_INCLUDE_FIXED_TYPES
# define NK_INCLUDE_STANDARD_IO
# define NK_INCLUDE_STANDARD_VARARGS
# define NK_INCLUDE_DEFAULT_ALLOCATOR
# define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
# define NK_INCLUDE_FONT_BAKING
# define NK_INCLUDE_DEFAULT_FONT
# define NK_IMPLEMENTATION
# define NK_SDL_GL3_IMPLEMENTATION
# include "nuklear.h"
# include "nuklear_sdl_gl3.h"
# include "Sdl_gl_win.hpp"

Sdl_gl_win::Sdl_gl_win(size_t width, size_t height) : width(width), height(height) {
    initSDL();
    initGL();
    
    keyMap[Event::PLAYER_LEFT] =    SDLK_a;
    keyMap[Event::PLAYER_RIGHT] =   SDLK_d;
    keyMap[Event::PLAYER_UP] =      SDLK_w;
    keyMap[Event::PLAYER_DOWN] =    SDLK_s;
    keyMap[Event::SPAWN_BOMB] =     SDLK_SPACE;
    
    keyMap[Event::CAMERA_LEFT] =    SDLK_LEFT;
    keyMap[Event::CAMERA_RIGHT] =   SDLK_RIGHT;
    keyMap[Event::CAMERA_UP] =      SDLK_UP;
    keyMap[Event::CAMERA_DOWN] =    SDLK_DOWN;
    keyMap[Event::RESET_CAMERA] =   SDLK_RETURN;
    
    keyMap[Event::DEBUG_MODE] =     SDLK_TAB;
    keyMap[Event::FOLLOW_PLAYER] =  SDLK_1;
    keyMap[Event::GUI_BASE_MENU] =  SDLK_LCTRL;

    SEventManager & event = SEventManager::getInstance();
    event.registerEvent(Event::SCREEN_FORMAT_UPDATE, MEMBER_CALLBACK(Sdl_gl_win::updateScreenFormat));
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
                                        width, height,
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void    Sdl_gl_win::eventManager(std::vector<Action::Enum> & actions, struct nk_context * nk_ctx) {
    SEventManager & event = SEventManager::getInstance();
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
            event.raise(Event::KEYDOWN, &events.key.keysym.sym);
            Action::Enum a;
            SDL_Keycode sym = events.key.keysym.sym;

                     if (sym == keyMap[Event::PLAYER_LEFT]) {     a = Action::LEFT;           event.raise(Event::PLAYER_LEFT, nullptr); }
                else if (sym == keyMap[Event::PLAYER_RIGHT]) {    a = Action::RIGHT;          event.raise(Event::PLAYER_RIGHT, nullptr); }
                else if (sym == keyMap[Event::PLAYER_UP]) {       a = Action::UP;             event.raise(Event::PLAYER_UP, nullptr); }
                else if (sym == keyMap[Event::PLAYER_DOWN]) {     a = Action::DOWN;           event.raise(Event::PLAYER_DOWN, nullptr); }
                else if (sym == keyMap[Event::SPAWN_BOMB]) {      a = Action::SPAWN_BOMB;     event.raise(Event::SPAWN_BOMB, nullptr); }
                else if (sym == keyMap[Event::CAMERA_LEFT]) {     a = Action::CAMERA_LEFT;    event.raise(Event::CAMERA_LEFT, nullptr); }
                else if (sym == keyMap[Event::CAMERA_RIGHT]) {    a = Action::CAMERA_RIGHT;   event.raise(Event::CAMERA_RIGHT, nullptr); }
                else if (sym == keyMap[Event::CAMERA_UP]) {       a = Action::CAMERA_UP;      event.raise(Event::CAMERA_UP, nullptr); }
                else if (sym == keyMap[Event::CAMERA_DOWN]) {     a = Action::CAMERA_DOWN;    event.raise(Event::CAMERA_DOWN, nullptr); }
                else if (sym == keyMap[Event::RESET_CAMERA]) {    a = Action::RESET_CAMERA;   event.raise(Event::RESET_CAMERA, nullptr); }
                else if (sym == keyMap[Event::DEBUG_MODE]) {      event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::DEBUG)); }
                else if (sym == keyMap[Event::FOLLOW_PLAYER]) {   a = Action::FOLLOW_PLAYER;  event.raise(Event::FOLLOW_PLAYER, nullptr); }
                else if (sym == keyMap[Event::GUI_BASE_MENU]) {   event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::BASE)); }
                
			if (find(actions.begin(), actions.end(), a) == actions.end())
                actions.push_back(a);
            else if (a == Action::DEBUG_MODE)
		    	actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());            
        } 
        if (events.type == SDL_KEYUP) {
            event.raise(Event::KEYUP, &events.key.keysym.sym);            
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
                case SDLK_2:        a = Action::BOMB_EXPLODES; break;
            }
			actions.erase(std::remove(actions.begin(), actions.end(), a), actions.end());
        }
    }
    nk_input_end(nk_ctx);
}

void            Sdl_gl_win::updateScreenFormat(void *f) {
    Screen::Format  format = *static_cast<Screen::Format*>(f);
    int             w, h;
    switch (format.resolution) {
        case Screen::Resolution::RES_2560_1440: w = 2560; h = 1440; break;
        case Screen::Resolution::RES_1920_1080: w = 1920; h = 1080; break;
        case Screen::Resolution::RES_1024_768:  w = 1024; h = 768; break;
    }
    if (format.mode == Screen::Mode::FULLSCREEN) {
        SDL_DisplayMode fsmode;
        fsmode.w = w;
        fsmode.h = h;
        fsmode.format = SDL_PIXELFORMAT_ARGB8888;
        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
        SDL_SetWindowDisplayMode(win, &fsmode);
    }
    else {
        SDL_SetWindowFullscreen(win, 0);                
    }
    SDL_SetWindowSize(win, w, h);
    Shader::perspective = glm::perspective(glm::radians(FOV), static_cast<float>(w) / static_cast<float>(h), Z_NEAR, Z_FAR);
}

SDL_Window      *Sdl_gl_win::getWin() const {
    return win;
}

std::map<Event::Enum, SDL_Keycode>    Sdl_gl_win::getKeyMap() const {
    return keyMap;
}

void            Sdl_gl_win::setKeyMap(std::map<Event::Enum, SDL_Keycode> km) {
    keyMap = km;
}

int             Sdl_gl_win::getMouseX() const {
    return mouseX;
}

int             Sdl_gl_win::getMouseY() const {
    return mouseY;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:34:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/18 13:18:21 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Sdl_gl_win.hpp"

Sdl_gl_win::Sdl_gl_win(size_t width, size_t height) : width(width), height(height) {
    initSDL();
    initGL();
    
    keyMap[Event::HUMAN_PLAYER_LEFT] =    SDLK_a;
    keyMap[Event::HUMAN_PLAYER_RIGHT] =   SDLK_d;
    keyMap[Event::HUMAN_PLAYER_UP] =      SDLK_w;
    keyMap[Event::HUMAN_PLAYER_DOWN] =    SDLK_s;
    keyMap[Event::HUMAN_SPAWN_BOMB] =     SDLK_SPACE;
    
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

    event.registerEvent(Event::KEY_MAP_UPDATE, MEMBER_CALLBACK(Sdl_gl_win::updateKeyMap));
}

Sdl_gl_win::Sdl_gl_win(Sdl_gl_win const & src) {
    *this = src;
}

Sdl_gl_win& Sdl_gl_win::operator=(Sdl_gl_win const & rhs) {
    if (this != &rhs)
    {
        mouseX = rhs.getMouseX();
        mouseY = rhs.getMouseY();
        win = rhs.getWin();
        modes = rhs.getDisplayModes();
        keyMap = rhs.getKeyMap();
    }

    return *this;
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

    // Gets all available display resolutions
    int n = SDL_GetNumDisplayModes(0);
    SDL_DisplayMode mode;
    
    for (int i = 0; i < n; ++i) {
        SDL_GetDisplayMode(0, i, &mode);
        if (static_cast<float>(mode.w) / static_cast<float>(mode.h) == 16.f / 9.f)
            modes.push_back(mode);
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

void    Sdl_gl_win::eventManager(struct nk_context * nk_ctx) {
    SEventManager & event = SEventManager::getInstance();
    SDL_GetRelativeMouseState(&mouseX,&mouseY);
    nk_input_begin(nk_ctx);
    while (SDL_PollEvent(&events)) {
        if (events.window.event == SDL_WINDOWEVENT_CLOSE ||
            (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE)) {
                event.raise(Event::QUIT_GAME, nullptr);
        }
        nk_sdl_handle_event(&events);
        if (events.type == SDL_KEYDOWN) {
            event.raise(Event::KEYDOWN, &events.key.keysym.sym);
            SDL_Keycode sym = events.key.keysym.sym;
                     if (sym == keyMap[Event::HUMAN_PLAYER_LEFT])   { event.raise(Event::HUMAN_PLAYER_LEFT, nullptr); }
                else if (sym == keyMap[Event::HUMAN_PLAYER_RIGHT])  { event.raise(Event::HUMAN_PLAYER_RIGHT, nullptr); }
                else if (sym == keyMap[Event::HUMAN_PLAYER_UP])     { event.raise(Event::HUMAN_PLAYER_UP, nullptr); }
                else if (sym == keyMap[Event::HUMAN_PLAYER_DOWN])   { event.raise(Event::HUMAN_PLAYER_DOWN, nullptr); }
                else if (sym == keyMap[Event::HUMAN_SPAWN_BOMB])    { event.raise(Event::HUMAN_SPAWN_BOMB, nullptr); }
                else if (sym == keyMap[Event::CAMERA_LEFT])         { event.raise(Event::CAMERA_LEFT, nullptr); }
                else if (sym == keyMap[Event::CAMERA_RIGHT])        { event.raise(Event::CAMERA_RIGHT, nullptr); }
                else if (sym == keyMap[Event::CAMERA_UP])           { event.raise(Event::CAMERA_UP, nullptr); }
                else if (sym == keyMap[Event::CAMERA_DOWN])         { event.raise(Event::CAMERA_DOWN, nullptr); }
                else if (sym == keyMap[Event::RESET_CAMERA])        { event.raise(Event::RESET_CAMERA, nullptr); }
                else if (sym == keyMap[Event::DEBUG_MODE])          { event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::DEBUG)); }
                else if (sym == keyMap[Event::FOLLOW_PLAYER])       { event.raise(Event::FOLLOW_PLAYER, nullptr); }
                else if (sym == keyMap[Event::GUI_BASE_MENU])       { event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::BASE)); }
                
        } 
        if (events.type == SDL_KEYUP) {
            event.raise(Event::KEYUP, &events.key.keysym.sym);            
                     if (events.key.keysym.sym == keyMap[Event::HUMAN_PLAYER_LEFT])     { event.raise(Event::END_HUMAN_PLAYER_LEFT, nullptr); }
                else if (events.key.keysym.sym == keyMap[Event::HUMAN_PLAYER_RIGHT])    { event.raise(Event::END_HUMAN_PLAYER_RIGHT, nullptr); }
                else if (events.key.keysym.sym == keyMap[Event::HUMAN_PLAYER_UP])       { event.raise(Event::END_HUMAN_PLAYER_UP, nullptr); }
                else if (events.key.keysym.sym == keyMap[Event::HUMAN_PLAYER_DOWN])     { event.raise(Event::END_HUMAN_PLAYER_DOWN, nullptr); }
                else if (events.key.keysym.sym == keyMap[Event::HUMAN_SPAWN_BOMB])      { event.raise(Event::END_HUMAN_SPAWN_BOMB, nullptr); }
         
        }
    }
    nk_input_end(nk_ctx);
}

void            Sdl_gl_win::updateScreenFormat(void *f) {
    Screen::Format  format = *static_cast<Screen::Format*>(f);
    int             w, h, fs;
    w = format.displayMode.w;
    h = format.displayMode.h;
    fs = (format.windowMode == Screen::WindowMode::FULLSCREEN) ? SDL_WINDOW_FULLSCREEN : 0; 
    SDL_SetWindowSize(win, w, h);
    SDL_SetWindowDisplayMode(win, &format.displayMode);
    SDL_SetWindowFullscreen(win, fs);
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

std::vector<SDL_DisplayMode> const &  Sdl_gl_win::getDisplayModes() const {
    return modes;
}

void            Sdl_gl_win::updateKeyMap(void *k){
    std::map<Event::Enum, SDL_Keycode>  *km = static_cast<std::map<Event::Enum, SDL_Keycode>  *>(k);
    for (auto &&i : *km){
        keyMap[i.first] = i.second;
    }
}
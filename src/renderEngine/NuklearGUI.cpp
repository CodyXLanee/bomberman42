/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:26:16 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/04 14:55:47 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NuklearGUI.hpp"

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

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

NuklearGUI::NuklearGUI(SDL_Window *sdlWindow, Camera & camera) :
    win(sdlWindow), camera(camera),
    menuWidth(500), menuHeight(500), optionHeight(30),
    debug(false), menu(false), options(false), keyBindings(false) {
    ctx = nk_sdl_init(win);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    screenFormat = {
        Screen::Resolution::RES_1920_1080,
        Screen::Mode::WINDOWED
    };

    SEventManager & event = SEventManager::getInstance();
    event.registerEvent(Event::KEYDOWN, MEMBER_CALLBACK(NuklearGUI::handleKey));
    event.registerEvent(Event::TOGGLE_MENU, MEMBER_CALLBACK(NuklearGUI::toggle));
    event.registerEvent(Event::TOGGLE_OPTIONS, MEMBER_CALLBACK(NuklearGUI::toggle));
    event.registerEvent(Event::TOGGLE_KEYBINDINGS, MEMBER_CALLBACK(NuklearGUI::toggle));
}

NuklearGUI::~NuklearGUI() {
    nk_sdl_shutdown();    
}

struct nk_context * NuklearGUI::getContext () const { return ctx; }

void    NuklearGUI::toggle(void *p) {
    bool    *b = static_cast<bool*>(p);
    *b = !(*b); 
}

void    NuklearGUI::handleKey(void * p) {
    int key = *static_cast<int*>(p);
    switch (key) {
        case SDLK_TAB:      toggle(&debug); break;
        case SDLK_LCTRL:    toggle(&menu); break;
        default: break;
    }
}

void    NuklearGUI::render() {    
    if (debug) {
        renderDebug();
    }  
    if (keyBindings) {
        renderKeyBindings();
    }
    else if (options) {
        renderOptions();
    }
    else if (menu) {
        renderMenu();
    }
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void    NuklearGUI::renderKeyBindings() {
    int  w, h;
    SDL_GetWindowSize(win, &w, &h);
    SEventManager & event = SEventManager::getInstance();    

    if (nk_begin(ctx, "KEY BINDINGS", nk_rect(w / 2 - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight),
    NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move up", NK_TEXT_LEFT);
        nk_button_label(ctx, "");

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move down", NK_TEXT_LEFT);
        nk_button_label(ctx, "");

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move left", NK_TEXT_LEFT);
        nk_button_label(ctx, "");

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move right", NK_TEXT_LEFT);
        nk_button_label(ctx, "");

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Drop bomb", NK_TEXT_LEFT);
        nk_button_label(ctx, "");

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        if (nk_button_label(ctx, "Apply"))
        {
            event.raise(Event::TOGGLE_KEYBINDINGS, &keyBindings);
        }
        if (nk_button_label(ctx, "Back"))
        {
            event.raise(Event::TOGGLE_KEYBINDINGS, &keyBindings);
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderOptions() {

    int  w, h;
    SDL_GetWindowSize(win, &w, &h);
    SEventManager & event = SEventManager::getInstance();

    static float        masterVolume = 0.8f;
    static float        musicVolume = 0.5f;
    static float        effectsVolume = 0.6f;

    static Screen::Format   displayedFormat = screenFormat;    
   // displayedFormat.resolution = screenResolution;
    //displayedFormat.mode = screenMode;
    
    std::string screenResString = toString(displayedFormat.resolution);
    std::string screenModeString = toString(displayedFormat.mode);

    if (nk_begin(ctx, "OPTIONS", nk_rect(w / 2 - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {
        nk_menubar_begin(ctx);
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen resolution", NK_TEXT_LEFT);
        if (nk_menu_begin_label(ctx, screenResString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight, 1);
            
            if (nk_menu_item_label(ctx, "2560 x 1440", NK_TEXT_CENTERED)) {
                displayedFormat.resolution = Screen::Resolution::RES_2560_1440;
            }
            if (nk_menu_item_label(ctx, "1920 x 1080", NK_TEXT_CENTERED)) {
                displayedFormat.resolution = Screen::Resolution::RES_1920_1080;
            }
            if (nk_menu_item_label(ctx, "1024 x 768", NK_TEXT_CENTERED)) {
                displayedFormat.resolution = Screen::Resolution::RES_1024_768;
            }
            nk_menu_end(ctx);
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen mode", NK_TEXT_LEFT);     
        if (nk_menu_begin_label(ctx, screenModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {

            nk_layout_row_dynamic(ctx, optionHeight, 1);
            if (nk_menu_item_label(ctx, "WINDOWED", NK_TEXT_CENTERED)) {
                displayedFormat.mode = Screen::Mode::WINDOWED;
            }
            if (nk_menu_item_label(ctx, "FULLSCREEN", NK_TEXT_CENTERED)) {
                displayedFormat.mode = Screen::Mode::FULLSCREEN;
            }
            nk_menu_end(ctx);
        }
        nk_menubar_end(ctx);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Master volume", NK_TEXT_LEFT);             
        nk_slider_float(ctx, 0, &masterVolume, 1.0f, 0.1f);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Music volume", NK_TEXT_LEFT);             
        nk_slider_float(ctx, 0, &musicVolume, 1.0f, 0.1f);
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Effects volume", NK_TEXT_LEFT);             
        nk_slider_float(ctx, 0, &effectsVolume, 1.0f, 0.1f);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Key bindings", NK_TEXT_LEFT);     
        if (nk_button_label(ctx, "Configure"))
        {
            event.raise(Event::TOGGLE_KEYBINDINGS, &keyBindings);                        
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Apply"))
        {
            if (screenFormat.resolution != displayedFormat.resolution || screenFormat.mode != displayedFormat.mode) {
                screenFormat.resolution = displayedFormat.resolution;
                screenFormat.mode = displayedFormat.mode;
                displayedFormat.resolution = screenFormat.resolution;
                displayedFormat.mode = screenFormat.mode;
                event.raise(Event::SCREEN_FORMAT_UPDATE, &displayedFormat);                        
            }
            event.raise(Event::TOGGLE_OPTIONS, &options);                        
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Back"))
        {
            displayedFormat.resolution = screenFormat.resolution;
            displayedFormat.mode = screenFormat.mode;
            event.raise(Event::TOGGLE_OPTIONS, &options);                        
        }
        
    }
    nk_end(ctx);
}

void    NuklearGUI::renderMenu() {
    int  w, h;
    SDL_GetWindowSize(win, &w, &h);
    SEventManager & event = SEventManager::getInstance();
    if (nk_begin(ctx, "MENU", nk_rect(w / 2 - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Resume"))
        {
            event.raise(Event::TOGGLE_MENU, &menu);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Options"))
        {
            event.raise(Event::TOGGLE_OPTIONS, &options);            
        }
   
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Quit"))
        {
            event.raise(Event::QUIT_GAME, nullptr);            
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderDebug() {
    glm::vec3   camPos = camera.getPosition();
    glm::vec3   camFront = camera.getFront();
    std::string camPosString = "Camera position: " + std::to_string(camPos.x) + " : " + std::to_string(camPos.y) + " : " + std::to_string(camPos.z);
    std::string camFrontString = "Camera front: " + std::to_string(camFront.x) + " : " + std::to_string(camFront.y) + " : " + std::to_string(camFront.z);


     if (nk_begin(ctx, "DEBUG MODE", nk_rect(50, 50, 400, 100),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        nk_layout_row_begin(ctx, NK_STATIC, 30, 1);
        {
            nk_layout_row_push(ctx, 350);
            nk_label(ctx, camPosString.c_str(), NK_TEXT_LEFT);
        }
        nk_layout_row_end(ctx);
        nk_layout_row_begin(ctx, NK_STATIC, 30, 1);
        {
            nk_layout_row_push(ctx, 350);
            nk_label(ctx, camFrontString.c_str(), NK_TEXT_LEFT);
        }
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);
}

std::string     NuklearGUI::toString(Screen::Resolution r) const {
    std::string res;
    switch (r) {
        case Screen::Resolution::RES_2560_1440: res = "2560 x 1440"; break;
        case Screen::Resolution::RES_1920_1080: res = "1920 x 1080"; break;
        case Screen::Resolution::RES_1024_768:  res = "1024 x 768"; break;
    }
    return res;
}

std::string     NuklearGUI::toString(Screen::Mode m) const {
    std::string mode;
    switch (m) {
        case Screen::Mode::WINDOWED:  mode = "WINDOWED"; break;
        case Screen::Mode::FULLSCREEN:mode = "FULLSCREEN"; break;
    }
    return mode;
}
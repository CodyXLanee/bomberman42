/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:26:16 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/04 13:12:17 by egaborea         ###   ########.fr       */
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

NuklearGUI::NuklearGUI(SDL_Window *sdlWindow) :
    win(sdlWindow), menuWidth(500), menuHeight(500), optionHeight(30) {
    ctx = nk_sdl_init(win);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    screenResolution = Screen::Resolution::RES_1920_1080;
    screenMode = Screen::Mode::WINDOWED;
}

NuklearGUI::~NuklearGUI() {
    nk_sdl_shutdown();    
}

struct nk_context * NuklearGUI::getContext () const { return ctx; }

void    NuklearGUI::render(std::vector<Action::Enum> & actions, Camera & camera) {    
    if (find(actions.begin(), actions.end(), Action::DEBUG_MODE) != actions.end()) {
        renderDebug(camera);
    }
    if (find(actions.begin(), actions.end(), Action::MENU) != actions.end()) {
        if (find(actions.begin(), actions.end(), Action::OPTIONS) != actions.end()) {
            if (find(actions.begin(), actions.end(), Action::KEY_BINDINGS) != actions.end()) {
                renderKeyBindings(actions);
            }
            renderOptions(actions);
        }
        else {
            renderMenu(actions);
        }
    }
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void    NuklearGUI::renderKeyBindings(std::vector<Action::Enum> & actions) {
    int  w, h;
    SDL_GetWindowSize(win, &w, &h);

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
            actions.erase(std::remove(actions.begin(), actions.end(), Action::KEY_BINDINGS), actions.end());            
        }
        if (nk_button_label(ctx, "Back"))
        {
            actions.erase(std::remove(actions.begin(), actions.end(), Action::KEY_BINDINGS), actions.end());            
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderOptions(std::vector<Action::Enum> & actions) {

    int  w, h;
    SDL_GetWindowSize(win, &w, &h);

    static float        masterVolume = 0.8f;
    static float        musicVolume = 0.5f;
    static float        effectsVolume = 0.6f;
    
    static Screen::Resolution  displayedRes = screenResolution;
    static Screen::Mode        displayedMode = screenMode;
    std::string screenResString = toString(displayedRes);
    std::string screenModeString = toString(displayedMode);

    if (nk_begin(ctx, "OPTIONS", nk_rect(w / 2 - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {
        nk_menubar_begin(ctx);
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen resolution", NK_TEXT_LEFT);
        if (nk_menu_begin_label(ctx, screenResString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight, 1);
            
            if (nk_menu_item_label(ctx, "2560 x 1440", NK_TEXT_CENTERED)) {
                displayedRes = Screen::Resolution::RES_2560_1440;
            }
            if (nk_menu_item_label(ctx, "1920 x 1080", NK_TEXT_CENTERED)) {
                displayedRes = Screen::Resolution::RES_1920_1080;
            }
            if (nk_menu_item_label(ctx, "1024 x 768", NK_TEXT_CENTERED)) {
                displayedRes = Screen::Resolution::RES_1024_768;
            }
            nk_menu_end(ctx);
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen mode", NK_TEXT_LEFT);     
        if (nk_menu_begin_label(ctx, screenModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {

            nk_layout_row_dynamic(ctx, optionHeight, 1);
            if (nk_menu_item_label(ctx, "WINDOWED", NK_TEXT_CENTERED)) {
                displayedMode = Screen::Mode::WINDOWED;
            }
            if (nk_menu_item_label(ctx, "FULLSCREEN", NK_TEXT_CENTERED)) {
                displayedMode = Screen::Mode::FULLSCREEN;
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
            /* Key bindings */
            actions.push_back(Action::KEY_BINDINGS);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Apply"))
        {
            if (screenResolution != displayedRes || screenMode != displayedMode) {
                screenResolution = displayedRes;
                screenMode = displayedMode;
                actions.push_back(Action::SCREEN_CHANGED);
            }
            actions.erase(std::remove(actions.begin(), actions.end(), Action::OPTIONS), actions.end());            
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Back"))
        {
            displayedRes = screenResolution;
            displayedMode = screenMode;
            actions.erase(std::remove(actions.begin(), actions.end(), Action::OPTIONS), actions.end());            
        }
        
    }
    nk_end(ctx);
}

void    NuklearGUI::renderMenu(std::vector<Action::Enum> & actions) {
    int  w, h;
    SDL_GetWindowSize(win, &w, &h);
    if (nk_begin(ctx, "MENU", nk_rect(w / 2 - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Resume"))
        {		    	
            actions.erase(std::remove(actions.begin(), actions.end(), Action::MENU), actions.end());            
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Options"))
        {
            actions.push_back(Action::OPTIONS);
        }
   
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Quit"))
        {
            actions.clear();
            actions.push_back(Action::ESCAPE);
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderDebug(Camera & camera) {
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

Screen::Resolution  NuklearGUI::getScreenResolution() const { return screenResolution; }
Screen::Mode        NuklearGUI::getScreenMode() const { return screenMode; }
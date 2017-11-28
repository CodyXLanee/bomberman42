/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:26:16 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/28 14:49:40 by lfourque         ###   ########.fr       */
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

NuklearGUI::NuklearGUI(SDL_Window *sdlWindow) {      
    ctx = nk_sdl_init(sdlWindow);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
}

NuklearGUI::~NuklearGUI() {
    nk_sdl_shutdown();    
}

struct nk_context * NuklearGUI::getContext () const { return ctx; }

void    NuklearGUI::render(std::vector<Action::Enum> const & actions, Camera & camera) {  
    if (find(actions.begin(), actions.end(), Action::DEBUG_MODE) != actions.end()) {
        renderDebug(camera);
    }
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void    NuklearGUI::renderDebug(Camera & camera) {
    glm::vec3   camPos = camera.getPosition();
    std::string camPosString = "Camera position: " + std::to_string(camPos.x) + " : " + std::to_string(camPos.y) + " : " + std::to_string(camPos.z);

     if (nk_begin(ctx, "DEBUG MODE", nk_rect(50, 50, 400, 100),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        nk_layout_row_begin(ctx, NK_STATIC, 30, 1);
        {
            nk_layout_row_push(ctx, 350);
            nk_label(ctx, camPosString.c_str(), NK_TEXT_LEFT);
        }
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/07 16:55:16 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUKLEAR_GUI_HPP
# define NUKLEAR_GUI_HPP

# include <SDL.h>
# include <SDL_mixer.h>
# include <vector>
# include <stack>
# include "bomberman.hpp"
# include "Camera.hpp"
# include "SEventManager.hpp"
# include "Sdl_gl_win.hpp"
# include "nuklear.h"
# include "nuklear_sdl_gl3.h"

class NuklearGUI
{
    public:
        NuklearGUI(Sdl_gl_win &, Camera &);
        ~NuklearGUI();

        struct nk_context *     getContext() const;
        Screen::Resolution      getScreenResolution() const;
        Screen::Mode            getScreenMode() const;
        void                    render();

        std::string             toString(Screen::Resolution) const;
        std::string             toString(Screen::Mode) const;
        
    private:
        Sdl_gl_win              &win;
        Camera                  &camera;
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        int                     const menuWidth;
        int                     const menuHeight;
        int                     const optionHeight;

        Screen::Format          screenFormat;

        std::stack<Menu::Enum>              _active_menu;
        std::map<Event::Enum, SDL_Keycode>  _keysMap;
        SDL_Keycode                       * _keyToChange;

        void        handleKey(void *);
        void        toggle(void *);
        void        bindKeyToEvent(Event::Enum, std::map<Event::Enum, SDL_Keycode> &);

        void        renderDebug();
        void        renderMenu();
        void        renderOptions();
        void        renderKeyBindings();
        void        renderStartMenu();

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/15 15:12:56 by lfourque         ###   ########.fr       */
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
# include <stb_image.h>


class NuklearGUI
{
    public:
        NuklearGUI(Sdl_gl_win &, Camera &);
        ~NuklearGUI();

        struct nk_context *     getContext() const;
        Screen::Resolution      getScreenResolution() const;
        Screen::Mode            getScreenMode() const;
        void                    render();
        
    private:
        Sdl_gl_win              &win;
        Camera                  &camera;
        SEventManager           &event;
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        
        float                   menuWidth;
        float                   menuHeight;
        int                     windowWidth;
        int                     windowHeight;
        float                   optionHeight;

        std::chrono::time_point<std::chrono::steady_clock>          start_time;
        float                                                       fps;
        int                                                         frames;
        void                                                        update_fps(void);

        Screen::Format          screenFormat;

        std::stack<Menu::Enum>              _active_menu;
        std::map<Event::Enum, SDL_Keycode>  _keysMap;
        SDL_Keycode                       * _keyToChange;

        void        handleKey(void *);
        void        toggle(void *);
        void        bindKeyToEvent(Event::Enum, std::map<Event::Enum, SDL_Keycode> &);
        void        hover(int) const;

        void        renderDebug();
        void        renderMenu();
        void        renderOptions();
        void        renderKeyBindings();
        void        renderLevelSelection();
        void        renderStartMenu();
        void        renderHUD();

        struct nk_image         loadImage(std::string const, GLint);        

        std::string             toString(Screen::Resolution) const;
        std::string             toString(Screen::Mode) const;
        std::string             toString(Camera::Mode) const;

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
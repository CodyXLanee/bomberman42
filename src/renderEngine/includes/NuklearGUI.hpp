/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/18 12:30:04 by lfourque         ###   ########.fr       */
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
      //  Screen::Resolution      getScreenResolution() const;
      //  Screen::Mode            getScreenMode() const;
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

        float                   _masterVolume;
        float                   _effectsVolume;
        float                   _musicVolume;

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
        void        renderGameModeSelectionMenu();
        void        renderStartMenu();
        void        renderHUD();

        void        setMasterVolume(void * v);
        void        setEffectsVolume(void * v);
        void        setMusicVolume(void * v);
        void        updateScreenFormat(void *f);

        struct nk_image         loadImage(std::string const, GLint);        

        std::string             toString(SDL_DisplayMode const &) const;
        std::string             toString(Screen::WindowMode) const;
        std::string             toString(Camera::Mode) const;

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
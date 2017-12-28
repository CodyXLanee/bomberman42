/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/21 17:00:33 by lfourque         ###   ########.fr       */
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
# include "GameParams.hpp"
# include "Player.hpp"
# include "Bonus.hpp"
# include <stb_image.h>


class NuklearGUI
{
    public:
        NuklearGUI(Sdl_gl_win &, Camera &);
        ~NuklearGUI();

        struct nk_context *     getContext() const;
        void                    render(bool);
        
        void        setStarsCampaign(std::vector<int> starsCampaign);
        
    private:
        Sdl_gl_win              &win;
        Camera                  &camera;
        SEventManager           &event;
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        struct nk_font          *bigFont;
        struct nk_font          *mediumFont;
        struct nk_font          *smallFont;
        
        float                   menuWidth;
        float                   menuHeight;
        int                     windowWidth;
        int                     windowHeight;
        float                   optionHeight;
        bool                    screenFormatUpdate;
        
        float                   _masterVolume;
        float                   _effectsVolume;
        float                   _musicVolume;
        std::vector<int>        _starsCampaign;
        
        std::chrono::time_point<std::chrono::steady_clock>          start_time;
        float                                                       fps;
        int                                                         frames;
        void                                                        update_fps(void);
        
        Screen::Format          screenFormat;
        
        std::stack<Menu::Enum>              _active_menu;
        std::map<Event::Enum, SDL_Keycode>  _keysMap;
        SDL_Keycode                       * _keyToChange;

        PlayerColor::Enum                   _human_player_color;
        glm::ivec3                          _human_player_bonus;
        
        void        setupProportions();
        void        setupFont();
        void        handleKey(void *);
        void        toggle(void *);
        void        bindKeyToEvent(Event::Enum, std::map<Event::Enum, SDL_Keycode> &);
        void        hover(int) const;
        
        void        renderBackgroundImage();
        void        renderHUD();        
        void        renderDebug();
        void        renderMenu();
        void        renderOptions();
        void        renderKeyBindings();
        void        renderLevelSelection();
        void        renderGameModeSelectionMenu();
        void        renderNewBrawlMenu();
        void        renderStartMenu();
        void        renderSelectSlot(void);

        void        setMasterVolume(void * v);
        void        setEffectsVolume(void * v);
        void        setMusicVolume(void * v);
        void        updateScreenFormat(void *f);
        void        updateHumanPlayerBonus(void *p);

        struct nk_image         loadImage(std::string const, GLint);        

        std::string             toString(SDL_DisplayMode const &) const;
        std::string             toString(Screen::WindowMode) const;
        std::string             toString(Camera::Mode) const;
        std::string             toString(PlayerColor::Enum) const;
        std::string             toString(Difficulty::Enum) const;

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
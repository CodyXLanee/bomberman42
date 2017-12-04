/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/04 14:45:11 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUKLEAR_GUI_HPP
# define NUKLEAR_GUI_HPP

# include <SDL.h>
# include <vector>
# include "bomberman.hpp"
# include "Camera.hpp"
# include "SEventManager.hpp"

class NuklearGUI
{
    public:
        NuklearGUI(SDL_Window *, Camera &);
        ~NuklearGUI();

        struct nk_context *     getContext() const;
        Screen::Resolution      getScreenResolution() const;
        Screen::Mode            getScreenMode() const;
        void                    render();

        std::string             toString(Screen::Resolution) const;
        std::string             toString(Screen::Mode) const;
        
    private:
        SDL_Window              *win;
        Camera                  &camera;
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        int                     const menuWidth;
        int                     const menuHeight;
        int                     const optionHeight;

        Screen::Format          screenFormat;

        bool        debug, menu, options, keyBindings;

        void        handleKey(void *);
        void        toggle(void *);

        void        renderDebug();
        void        renderMenu();
        void        renderOptions();
        void        renderKeyBindings();

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/04 12:28:39 by lfourque         ###   ########.fr       */
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
        void                    render(std::vector<Action::Enum> &, Camera &);

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

        Screen::Resolution      screenResolution;
        Screen::Mode            screenMode;

        void        handleKeydown(void *);

        void        renderDebug(Camera &);
        void        renderMenu(std::vector<Action::Enum> &);
        void        renderOptions(std::vector<Action::Enum> &);
        void        renderKeyBindings(std::vector<Action::Enum> &);

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
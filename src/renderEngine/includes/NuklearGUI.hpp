/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/28 13:37:50 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUKLEAR_GUI_HPP
# define NUKLEAR_GUI_HPP

# include <SDL.h>
# include <vector>
# include "bomberman.hpp"
# include "Camera.hpp"

class NuklearGUI
{
    public:
        NuklearGUI(SDL_Window *);
        ~NuklearGUI();

        struct nk_context * getContext() const;
        void                render(std::vector<Action::Enum> const &, Camera &);
        
    private:
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        
        void        renderDebug(Camera &);

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif
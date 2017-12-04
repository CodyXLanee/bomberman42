/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/04 14:46:08 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SGAMEMANAGER_HPP
# define SGAMEMANAGER_HPP

#include "bomberman.hpp"
#include "bomberman.hpp"
#include "Sdl_gl_win.hpp"
#include "GameEngine.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"

class SGameManager {
public:

    static SGameManager &	getInstance();

    ~SGameManager();

    void            manage(void);
    void            newGame(GameMode::Enum); 
    void            loadGame(Save::Enum);
    void            saveGame(Save::Enum);

    // GameMode::Enum  getGameMode(void);

    // Load / Save Config params

private:
    SGameManager();
    SGameManager(SGameManager const&);   // Don't Implement.
    void operator=(SGameManager const&); // Don't implement

    Sdl_gl_win          _window;
    GameEngine          _game;
    Camera              _camera;
    NuklearGUI          _gui;
    RenderEngine        _renderer;

};

#endif
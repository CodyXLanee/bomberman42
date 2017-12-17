/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/16 18:16:04 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SGAMEMANAGER_HPP
# define SGAMEMANAGER_HPP

# include "bomberman.hpp"
# include "Sdl_gl_win.hpp"
# include "GameEngine.hpp"
# include "RenderEngine.hpp"
# include "SoundManager.hpp"
# include "Camera.hpp"
# include "Slot.hpp"

class SGameManager {
public:

    static SGameManager &	getInstance();

    ~SGameManager();

    void            manage(void);
    void            newGame(GameMode::Enum); 
    void            loadGame(Save::Enum);
    void            saveGame(Save::Enum);

    void            quit_game(void *);
    void            new_game(void *);
    // GameMode::Enum  getGameMode(void);

    // Load / Save Config params

private:
    SGameManager();
    SGameManager(SGameManager const&);   // Don't Implement.
    void operator=(SGameManager const&); // Don't implement
    
    
    void            game_finish(void *p);
    void            useSlot(void) const;

    Sdl_gl_win          _window;
    GameEngine          *_game;
    Camera              _camera;
    NuklearGUI          _gui;
    RenderEngine        _renderer;
    SoundManager        _sound;
    Slot                *_slot;

    bool                _dev_mode;
    bool                _game_is_active;
    bool                _quit_game;
    bool                _new_game;

};

#endif
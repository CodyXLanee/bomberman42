/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/05 16:59:51 by lfourque         ###   ########.fr       */
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
# include "GameParams.hpp"

class SGameManager {
public:

    static SGameManager &	getInstance();

    ~SGameManager();

    void            manage(void);
    void            newGame(GameMode::Enum); 
    void            loadGame(Save::Enum);
    void            saveGame(Save::Enum);

    Slot &          getSlot(int) const;

    void            quit_game(void *);
    void            new_game(void *);
    // GameMode::Enum  getGameMode(void);

    // Load / Save Config params

private:
    SGameManager();
    SGameManager(SGameManager const&);   // Don't Implement.
    void operator=(SGameManager const&); // Don't implement
    
    
    void            game_finish(void *p);
    void            useSlot(void);
    void            loadSlot(void *s);

    Sdl_gl_win          _window;
    GameEngine          *_game;
    Camera              _camera;
    NuklearGUI          _gui;
    RenderEngine        _renderer;
    SoundManager        _sound;
    Slot                *_slots[3];
    Slot                *_current_slot;

    bool                _dev_mode;
    bool                _game_is_active;
    bool                _quit_game;
    
    int                 _counter;

};

#endif
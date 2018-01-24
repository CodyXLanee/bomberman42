/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/24 11:03:42 by egaborea         ###   ########.fr       */
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
    // void            newGame(GameMode::Enum); 
    // void            loadGame(Save::Enum);
    // void            saveGame(Save::Enum);

    Slot &          getSlot(int) const;
    Slot &          getCurrentSlot() const;

    void            quit_game(void *);
    void            new_game(void *);

    bool            is_game_active() const;

    void            end_end_animation(void *);
    // GameMode::Enum  getGameMode(void);

    // Load / Save Config params

private:
    SGameManager();
    SGameManager(SGameManager const&);   // Don't Implement.
    void operator=(SGameManager const&); // Don't implement
    
    
    void            restart_game(void *);
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
    GameParams          *_current_game_params;

    bool                _dev_mode;
    bool                _game_is_active;
    bool                _quit_game;
    bool                _post_game_display;

};

#endif
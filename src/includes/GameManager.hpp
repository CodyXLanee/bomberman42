/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/03 17:40:28 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SGAMEMANAGER_HPP
# define SGAMEMANAGER_HPP

#include "bomberman.hpp"

class SGameManager {
public:

    static SGameManager &	SGameManager::getInstance();

    ~SGameManager();

    void            setScreenResolution();
    void            getScreenResolution() const;

    void            setScreenMode();
    void            getScreenMode();

    void            setKeyBinding(Action::Enum, SDL_Keycode);
    SDL_Keycode     getKeyBinding(Action::Enum);

    void            setVolume(float, VolumeType::Enum);
    float           getVolume(VolumeType::Enum);

    void            newGame(GameMode::Enum); 
    void            loadGame(Save::Enum);
    void            saveGame(Save::Enum);

    GameMode::Enum  getGameMode(void);

    // Load / Save Config params

private:
        SGameManager();
        SGameManager(SGameManager const&);   // Don't Implement.
        void operator=(SGameManager const&); // Don't implement

};

#endif
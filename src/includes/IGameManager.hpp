/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGameManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 12:52:00 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/01 14:52:34 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IGAMEMANAGER_HPP
# define IGAMEMANAGER_HPP

namespace GameMode {
    enum Enum {BRAWL, CAMPAIGN};
};

namespace Save {
    enum Enum {SLOT1, SLOT2, SLOT3};
};

namespace VolumeType {
    enum Enum {MASTER, MUSIC, SOUND};
};

class IGameManager {
public:
    virtual ~IGameManager() = {};

    virtual void            setScreenResolution();
    virtual void            getScreenResolution() const;

    virtual void            setScreenMode();
    virtual void            getScreenMode();

    virtual void            setKeyBinding(Action::Enum, SDL_Keycode);
    virtual SDL_Keycode     getKeyBinding(Action::Enum);

    virtual void            setVolume(float, VolumeType::Enum);
    virtual float           getVolume(VolumeType::Enum);

    virtual void            newGame(GameMode::Enum); 
    virtual void            loadGame(Save::Enum);
    virtual void            saveGame(Save::Enum);

    virtual GameMode::Enum  getGameMode(void);

    // Load / Save Config params
    
};

#endif
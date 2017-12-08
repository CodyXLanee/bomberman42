/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoundManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 14:34:49 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/08 15:21:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SoundManager.hpp"

SoundManager::SoundManager() : masterVolume(0.0f), musicVolume(MIX_MAX_VOLUME / 2), effectsVolume(MIX_MAX_VOLUME / 2) {
    if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, BYTES_PER_OUTPUT_SAMPLE) == -1) {
       throw std::runtime_error(Mix_GetError());
    }
    Mix_AllocateChannels(10);
    
    music = Mix_LoadMUS("assets/sounds/carnivalrides.ogg");
    boom = Mix_LoadWAV("assets/sounds/explosions_explode.wav");
    boom2 = Mix_LoadWAV("assets/sounds/explosions_explodemini.wav");

    updateVolume();
    
    SEventManager & event = SEventManager::getInstance();
    event.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SoundManager::playMusic));
    event.registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(SoundManager::playBoom));
    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setEffectsVolume));
}

SoundManager::~SoundManager() {
    Mix_FreeChunk(boom);
    Mix_FreeChunk(boom2);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void    SoundManager::playBoom(void *) {
    std::uniform_int_distribution<int> distribution(0,1);
    int b = distribution(randomGenerator);
    if (b)
        Mix_PlayChannel(-1, boom, 0);
    else
        Mix_PlayChannel(-1, boom2, 0);
}

void    SoundManager::playMusic(void *p) {
    GameMode::Enum  mode = *static_cast<GameMode::Enum*>(p);
    if (mode == GameMode::CAMPAIGN) {
        Mix_PlayMusic(music, -1);
    }
}

void    SoundManager::updateVolume() {
    Mix_VolumeMusic(masterVolume * musicVolume);
    Mix_VolumeChunk(boom, masterVolume * effectsVolume);
    Mix_VolumeChunk(boom2, masterVolume * effectsVolume);
}

void    SoundManager::setMasterVolume(void * v) {
    masterVolume = *static_cast<float*>(v);
    updateVolume();
}

void    SoundManager::setEffectsVolume(void * v) {
    effectsVolume = *static_cast<float*>(v);
    updateVolume();
}

void    SoundManager::setMusicVolume(void * v) {
    musicVolume = *static_cast<float*>(v);
    updateVolume();
}
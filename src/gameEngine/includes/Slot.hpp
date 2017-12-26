/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 14:59:59 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/17 16:43:34 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SLOT_HPP
# define SLOT_HPP

# include "bomberman.hpp"
# include "Loader.hpp"
# include "rapidjson/filewritestream.h"
# include <SDL.h>
# include <cstdio>
# include <map>
# include <string>

class Slot {
	
	public:
        Slot(Save::Enum save);
        ~Slot();

        std::string                         save_to_path(Save::Enum save);
        void                                load_campaign_max_level(rapidjson::Value *val);
        void                                load_stars_campaign(rapidjson::Value *val);
        void                                load_float_val(rapidjson::Value *val, float *float_val);
        void                                load_screen_format(rapidjson::Value *val);
        void                                load_key_map(rapidjson::Value *val);
        
        void                                save();

        int                                 get_campaign_max_level() const ;
        void                                set_campaign_max_level(int i);

        int                                 get_stars_campaign(unsigned int level) const ;
        void                                set_stars_campaign(unsigned int level, int stars);
        
        float                               get_master_volume() const ;
        float                               get_music_volume() const;
        float                               get_effects_volume() const ;

        void                                setMasterVolume(void * v);
        void                                setEffectsVolume(void * v);
        void                                setMusicVolume(void * v);

        void                                updateScreenFormat(void *f);
        void                                updateKeyMap(void *k);

        Screen::Format const                &get_screenFormat() const;
        void                                set_screenFormat(Screen::Format const &sf);

        std::map<Event::Enum, SDL_Keycode> const                &getKeyMap() const;

    private:
		Loader						        _loader;
        int                                 _campaignMaxLevel;
        float                               _masterVolume;
        float                               _musicVolume;
        float                               _effectsVolume;
        Screen::Format                      _screenFormat;
        Save::Enum                          _save;
		std::map<Event::Enum, SDL_Keycode>	_keyMap;
        std::vector<int>                    _stars_campaign;
};

#endif
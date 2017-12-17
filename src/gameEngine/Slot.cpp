/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 16:23:25 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/17 16:28:02 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Slot.hpp"

std::string     Slot::save_to_path(Save::Enum save){
    switch(save){
        case Save::SLOT1:   return std::string("saves/slot1.json");
        case Save::SLOT2:   return std::string("saves/slot2.json");
        case Save::SLOT3:   return std::string("saves/slot3.json");
    }
}

void            Slot::load_campaign_max_level(rapidjson::Value *val){
    if (!val || !val[0].IsInt())
		throw std::runtime_error("Slot loading fail.");
    _campaignMaxLevel = val[0].GetInt();
}

void            Slot::load_float_val(rapidjson::Value *val, float *float_val){
    if (!val || !val[0].IsFloat())
		throw std::runtime_error("Slot loading fail.");
    *float_val = val[0].GetFloat();
}

void            Slot::load_screen_format(rapidjson::Value *val){
    if (!val || !val[0].HasMember("resolution") || !val[0].HasMember("mode") || !val[0]["resolution"].IsInt() || !val[0]["mode"].IsInt())
		throw std::runtime_error("Slot loading fail.");
    _screenFormat.resolution = static_cast<Screen::Resolution>(val[0]["resolution"].GetInt());
    _screenFormat.mode = static_cast<Screen::Mode>(val[0]["mode"].GetInt());
}


void            Slot::load_key_map(rapidjson::Value *val){
    if (!val || !val->HasMember("events") || !val->HasMember("key") || !val[0]["events"].IsArray() || !val[0]["key"].IsArray() || val[0]["key"].Size() != val[0]["events"].Size())
		throw std::runtime_error("Slot loading fail.");
    for (rapidjson::SizeType i = 0; i < val[0]["key"].Size(); i++){
        if (!val[0]["key"][i].IsInt() || !val[0]["events"][i].IsInt())
		    throw std::runtime_error("Slot loading fail.");
        _keyMap[static_cast<Event::Enum>(val[0]["events"][i].GetInt())] = static_cast<SDL_Keycode>(val[0]["key"][i].GetInt());
    }
}


Slot::Slot(Save::Enum save) : _save(save){
    _loader.setPath(save_to_path(save));
    if (_loader.load() != 1){
		throw std::runtime_error(save_to_path(save).c_str());
	}
    load_campaign_max_level(_loader.getValue("campaign_max_level"));
    load_float_val(_loader.getValue("master_volume"), &_masterVolume);
    load_float_val(_loader.getValue("music_volume"), &_musicVolume);
    load_float_val(_loader.getValue("effects_volume"), &_effectsVolume);
    load_screen_format(_loader.getValue("screen_format"));
    load_key_map(_loader.getValue("keyMap"));

    SEventManager &event = SEventManager::getInstance();

    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setEffectsVolume));

    event.registerEvent(Event::SCREEN_FORMAT_UPDATE, MEMBER_CALLBACK(Slot::updateScreenFormat));

    event.registerEvent(Event::KEY_MAP_UPDATE, MEMBER_CALLBACK(Slot::updateKeyMap));
}

Slot::~Slot(){
}

void                                Slot::save(){
    rapidjson::Document d;

    d.SetObject();
    d.AddMember("campaign_max_level", rapidjson::Value(_campaignMaxLevel), d.GetAllocator());
    d.AddMember("master_volume", rapidjson::Value(_masterVolume), d.GetAllocator());
    d.AddMember("music_volume", rapidjson::Value(_musicVolume), d.GetAllocator());
    d.AddMember("effects_volume", rapidjson::Value(_effectsVolume), d.GetAllocator());
    
    rapidjson::Value s(rapidjson::kObjectType);

    s.AddMember("resolution", rapidjson::Value(_screenFormat.resolution), d.GetAllocator());
    s.AddMember("mode", rapidjson::Value(_screenFormat.mode), d.GetAllocator());
    d.AddMember("screen_format", s, d.GetAllocator());

    rapidjson::Value events(rapidjson::kArrayType);
    rapidjson::Value key(rapidjson::kArrayType);
    for (auto i : _keyMap){
        events.PushBack(rapidjson::Value(static_cast<int>(i.first)), d.GetAllocator());
        key.PushBack(rapidjson::Value(static_cast<int>(i.second)), d.GetAllocator());
    }

    rapidjson::Value keyMap(rapidjson::kObjectType);
    keyMap.AddMember("events", events, d.GetAllocator());
    keyMap.AddMember("key", key, d.GetAllocator());
    d.AddMember("keyMap", keyMap, d.GetAllocator());



    FILE* fp = fopen(save_to_path(_save).c_str(), "w");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    d.Accept(writer);
    
    fclose(fp);
}
void    Slot::setMasterVolume(void * v) {
    _masterVolume = *static_cast<float*>(v);
}

void    Slot::setEffectsVolume(void * v) {
    _effectsVolume = *static_cast<float*>(v);
}

void    Slot::setMusicVolume(void * v) {
    _musicVolume = *static_cast<float*>(v);
}

float                               Slot::get_master_volume() const {
    return _masterVolume;
}
float                               Slot::get_music_volume() const{
    return _musicVolume;
}
float                               Slot::get_effects_volume() const{
    return _effectsVolume;
}

Screen::Format const                &Slot::get_screenFormat() const{
    return _screenFormat;
}


std::map<Event::Enum, SDL_Keycode> const                &Slot::getKeyMap() const{
    return _keyMap;
}

void                                Slot::updateScreenFormat(void *f) {
    Screen::Format  *format = static_cast<Screen::Format*>(f);
    _screenFormat.resolution = format->resolution;
    _screenFormat.mode = format->mode;
}

void                                Slot::updateKeyMap(void *k){
    std::map<Event::Enum, SDL_Keycode>  *km = static_cast<std::map<Event::Enum, SDL_Keycode>  *>(k);
    for (auto &&i : *km){
        _keyMap[i.first] = i.second;
    }
}
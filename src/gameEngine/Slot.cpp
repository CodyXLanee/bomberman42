/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 16:23:25 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/05 16:26:31 by egaborea         ###   ########.fr       */
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

void            Slot::load_stars_campaign(rapidjson::Value *val){
    if (!val || !val[0].IsArray())
        throw std::runtime_error("Slot loading fail.");
    for (unsigned int i = 0 ; i < val[0].Size() ; i++)
    {
        if (val[0][i].IsInt())
            set_stars_campaign(i, val[0][i].GetInt());
    }
}

void            Slot::load_float_val(rapidjson::Value *val, float *float_val){
    if (!val || !val[0].IsFloat())
		throw std::runtime_error("Slot loading fail.");
    *float_val = val[0].GetFloat();
}

void            Slot::load_screen_format(rapidjson::Value *val){
    if (!val || !val[0].HasMember("resolution_width") || !val[0].HasMember("resolution_height") || !val[0].HasMember("mode") || !val[0]["resolution_width"].IsInt() || !val[0]["resolution_height"].IsInt() || !val[0]["mode"].IsInt())
        throw std::runtime_error("Slot loading fail.");
    SDL_DisplayMode dm;
    dm.w = val[0]["resolution_width"].GetInt();
    dm.h = val[0]["resolution_height"].GetInt();
    _screenFormat.displayMode = dm; 
    _screenFormat.windowMode = static_cast<Screen::WindowMode>(val[0]["mode"].GetInt());
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
    try {
        if (_loader.load() != 1){
            throw std::runtime_error(save_to_path(save).c_str());
        }
        load_stars_campaign(_loader.getValue("stars_campaign"));
        load_campaign_max_level(_loader.getValue("campaign_max_level"));
        load_float_val(_loader.getValue("master_volume"), &_masterVolume);
        load_float_val(_loader.getValue("music_volume"), &_musicVolume);
        load_float_val(_loader.getValue("effects_volume"), &_effectsVolume);
        load_screen_format(_loader.getValue("screen_format"));
        load_key_map(_loader.getValue("keyMap"));
    }
    catch (std::runtime_error &){
        std::cout << "Error while loading " << save_to_path(save) << std::endl;
    }

    SEventManager &event = SEventManager::getInstance();

    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(Slot::setEffectsVolume));

    event.registerEvent(Event::SCREEN_FORMAT_UPDATE, MEMBER_CALLBACK(Slot::updateScreenFormat));

    event.registerEvent(Event::KEY_MAP_UPDATE, MEMBER_CALLBACK(Slot::updateKeyMap));
}

Slot::~Slot(){
    SEventManager &event = SEventManager::getInstance();

    event.unRegisterEvent(Event::MASTER_VOLUME_UPDATE, this);
    event.unRegisterEvent(Event::MUSIC_VOLUME_UPDATE, this);
    event.unRegisterEvent(Event::EFFECTS_VOLUME_UPDATE, this);

    event.unRegisterEvent(Event::SCREEN_FORMAT_UPDATE, this);

    event.unRegisterEvent(Event::KEY_MAP_UPDATE, this);
}

void                                Slot::save(){
    rapidjson::Document d;

    d.SetObject();

    rapidjson::Value stars(rapidjson::kArrayType);
    for (unsigned int i = 0 ; i < _stars_campaign.size() ; i++){
        stars.PushBack(rapidjson::Value(static_cast<int>(_stars_campaign[i])), d.GetAllocator());
    }
    d.AddMember("stars_campaign", stars, d.GetAllocator());

    d.AddMember("campaign_max_level", rapidjson::Value(_campaignMaxLevel), d.GetAllocator());
    d.AddMember("master_volume", rapidjson::Value(_masterVolume), d.GetAllocator());
    d.AddMember("music_volume", rapidjson::Value(_musicVolume), d.GetAllocator());
    d.AddMember("effects_volume", rapidjson::Value(_effectsVolume), d.GetAllocator());
    
    rapidjson::Value s(rapidjson::kObjectType);

    s.AddMember("resolution_width", rapidjson::Value(_screenFormat.displayMode.w), d.GetAllocator());
    s.AddMember("resolution_height", rapidjson::Value(_screenFormat.displayMode.h), d.GetAllocator());
    s.AddMember("mode", rapidjson::Value(_screenFormat.windowMode), d.GetAllocator());
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

void                                Slot::use(){
    SEventManager &em = SEventManager::getInstance();

    em.raise(Event::SCREEN_FORMAT_UPDATE, &_screenFormat);

    em.raise(Event::KEY_MAP_UPDATE, &_keyMap);

    em.raise(Event::MASTER_VOLUME_UPDATE, &_masterVolume);

    em.raise(Event::MUSIC_VOLUME_UPDATE, &_musicVolume);
    
    em.raise(Event::EFFECTS_VOLUME_UPDATE, &_effectsVolume);

    em.raise(Event::UPDATE_ALL_CAMPAIGN_STARS, &_stars_campaign);
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

void    Slot::set_stars_campaign(unsigned int level, int stars)
{
    unsigned int size = _stars_campaign.size();

    if (level == size)
        _stars_campaign.push_back(stars);
    if (level < size)
        _stars_campaign[level] = stars;
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
int                                 Slot::get_stars_campaign(unsigned int level) const{
    if (level < _stars_campaign.size())
        return _stars_campaign[level];
    return -1;
}
std::vector<int>                    Slot::get_all_stars_campaign() const{
    return _stars_campaign;
}

Screen::Format const                &Slot::get_screenFormat() const{
    return _screenFormat;
}


std::map<Event::Enum, SDL_Keycode> const                &Slot::getKeyMap() const{
    return _keyMap;
}

void                                Slot::updateScreenFormat(void *f) {
    Screen::Format  *format = static_cast<Screen::Format*>(f);
    _screenFormat.displayMode = format->displayMode;
    _screenFormat.windowMode = format->windowMode;
}

void                                Slot::updateKeyMap(void *k){
    std::map<Event::Enum, SDL_Keycode>  *km = static_cast<std::map<Event::Enum, SDL_Keycode>  *>(k);
    for (auto &&i : *km){
        _keyMap[i.first] = i.second;
    }
}

Save::Enum                          Slot::get_save(){
    return _save;
}
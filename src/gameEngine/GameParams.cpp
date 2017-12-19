/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameParams.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 10:38:58 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/19 11:57:38 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameParams.hpp"

GameParams::GameParams(GameMode::Enum) : _map_path(""){
}

GameParams::GameParams(GameMode::Enum game_mode, Level::Enum level, PlayerColor::Enum color, int enemies_nb, Difficulty::Enum difficulty) : _map_path(""){
        _game_mode = game_mode;
        _brawl_enemy_nb = enemies_nb;
        _level = level;
        _color = color;
        _difficulty = difficulty;
}

GameParams::GameParams(GameParams const & gp) : _map_path(gp.get_map_path()),
        _game_mode(gp.get_game_mode()),
        _brawl_enemy_nb(gp.get_brawl_enemy_nb()),
        _level(gp.get_level()),
        _color(gp.get_color()),
        _difficulty(gp.get_difficulty()){
}


GameParams::~GameParams(){
    
}

std::string const   &GameParams::get_map_path() const {
    return _map_path;
}

void                GameParams::set_map_path(std::string const & s){
    _map_path = s;
}

GameMode::Enum      GameParams::get_game_mode() const {
    return _game_mode;
}

void                GameParams::set_game_mode(GameMode::Enum gm){
    _game_mode = gm;
}

int                 GameParams::get_brawl_enemy_nb() const {
    return _brawl_enemy_nb;
}

void                GameParams::set_brawl_enemy_nb(int i){
    _brawl_enemy_nb = std::max(((i - 1) % 3) + 1, 1);
}

Level::Enum         GameParams::get_level() const {
    return _level;
}

void                GameParams::set_level(Level::Enum l){
    _level = l;
}

PlayerColor::Enum   GameParams::get_color() const {
    return _color;
}

void                GameParams::set_color(PlayerColor::Enum c){
    _color = c;
}

Difficulty::Enum    GameParams::get_difficulty() const {
    return _difficulty;
}

void                GameParams::set_difficulty(Difficulty::Enum d){
    _difficulty = d;
}
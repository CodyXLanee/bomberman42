/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameParams.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 14:52:23 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/19 11:57:26 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEPARAMS_HPP
# define GAMEPARAMS_HPP

# include "bomberman.hpp"
# include <string>

class GameParams {
	
	public:
		GameParams(GameMode::Enum);
        GameParams(GameMode::Enum game_mode, Level::Enum level, PlayerColor::Enum color, int enemies_nb, Difficulty::Enum difficulty);
        GameParams(GameParams const & gp);
		~GameParams();

        std::string const   &get_map_path() const ;
        void                set_map_path(std::string const & s);
        GameMode::Enum      get_game_mode() const ;
        void                set_game_mode(GameMode::Enum);
        int                 get_brawl_enemy_nb() const ;
        void                set_brawl_enemy_nb(int);
        Level::Enum         get_level() const ;
        void                set_level(Level::Enum);
        PlayerColor::Enum   get_color() const ;
        void                set_color(PlayerColor::Enum);
        Difficulty::Enum    get_difficulty() const ;
        void                set_difficulty(Difficulty::Enum);

    private:
		GameParams();
        std::string         _map_path;
        GameMode::Enum      _game_mode;
        int                 _brawl_enemy_nb;
        Level::Enum         _level;
        PlayerColor::Enum   _color;
        Difficulty::Enum    _difficulty;
        
};

#endif
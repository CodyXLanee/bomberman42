/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameParams.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 14:52:23 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/16 14:58:34 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEPARAMS_HPP
# define GAMEPARAMS_HPP

# include "bomberman.hpp"

namespace   PlayerType {
    enum Enum { HUMAN, AI, NONE}
}

class GameParams {
	
	public:
		GameParams();
		~GameParams();
    private:
        std::string         map_path;
        GameMode::Enum      game_mode;
        PlayerType::Enum    players[4];
        
};

#endif
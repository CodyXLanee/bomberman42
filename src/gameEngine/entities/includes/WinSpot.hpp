/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinSpot.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 18:21:35 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/03 15:55:07 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINSPOT_HPP
# define WINSPOT_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"

class WinSpot : public AGameEntity {
public:
    WinSpot(const glm::vec2 & pos);
    ~WinSpot();

private:
    void    check_player_pos(void *p);
};

#endif
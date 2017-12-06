/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:45:55 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 14:48:20 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"

class Player : public AGameEntity {
public:
    Player(const glm::vec2 & pos, int nb);

    glm::vec2	getGraphicalDirection() const;
    void		setGraphicalDirection(glm::vec2 dir);
    glm::vec2	getNewDirection(void);

    int         getPlayerNb(void);

    void		newDirLeft(void);
    void		newDirRight(void);
    void		newDirUp(void);
    void		newDirDown(void);
    void		endNewDirLeft(void);
    void		endNewDirRight(void);
    void		endNewDirUp(void);
    void		endNewDirDown(void);

private:
    int         _player_number;
    bool        left, right, up, down;
	glm::vec2	_graphicalDirection;
};

#endif
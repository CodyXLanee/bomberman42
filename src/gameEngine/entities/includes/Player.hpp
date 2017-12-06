/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:45:55 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 11:57:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"

class Player : public AGameEntity {
public:
    Player(const glm::vec2 & pos);

    glm::vec2	getGraphicalDirection() const;
    void		setGraphicalDirection(glm::vec2 dir);
    glm::vec2	getNewDirection(void);

private:

    void		newDirLeft(void *);
    void		newDirRight(void *);
    void		newDirUp(void *);
    void		newDirDown(void *);
    void		endNewDirLeft(void *);
    void		endNewDirRight(void *);
    void		endNewDirUp(void *);
    void		endNewDirDown(void *);
    bool        left, right, up, down;
	glm::vec2	_graphicalDirection;
};

#endif
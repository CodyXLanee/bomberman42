/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEntity.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 15:01:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 03:40:22 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEENTITY_HPP
# define GAMEENTITY_HPP

# include "IGameEntity.hpp"

class GameEntity : public IGameEntity {
public:
    GameEntity(const Type::Enum & type);
    GameEntity(const Type::Enum & type, const glm::vec2 & pos);
    virtual ~GameEntity();
    
    virtual glm::vec2	getPosition() const;
    virtual glm::vec2	getDirection() const;
    virtual State::Enum	getState() const;
    virtual float		getSpeed() const;
    virtual Type::Enum	getType() const;

    virtual void    	setPosition(const glm::vec2 &);
    virtual void    	setDirection(const glm::vec2 &);
    virtual void    	setState(const State::Enum &);
    virtual void    	setSpeed(const float &);
    virtual void    	setType(const Type::Enum &);
private:
    GameEntity();
    glm::vec2           _position;
    glm::vec2           _direction;
    State::Enum         _state;
    float               _speed;
    Type::Enum          _type;
};

#endif
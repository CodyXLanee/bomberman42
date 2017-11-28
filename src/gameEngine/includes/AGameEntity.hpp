/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:28:44 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/28 14:00:39 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AGAMEENTITY_HPP
# define AGAMEENTITY_HPP

# include "IGameEntity.hpp"

class AGameEntity : public IGameEntity {
public:
    virtual ~AGameEntity();
    virtual glm::vec2	getPosition() const;
    virtual glm::vec2	getDirection() const;
    virtual State::Enum	getState() const;
    virtual float		getSpeed() const;
    virtual Type::Enum	getType() const;

    virtual void    	setPosition(const glm::vec2 &);
    virtual void    	setDirection(const glm::vec2 &);
    virtual void    	setState(const State::Enum &);
    virtual void    	setSpeed(const float &);

    virtual void        update(void);
protected:
    AGameEntity(const glm::vec2 &, const glm::vec2 &, const State::Enum &, const float &, const Type::Enum &);
    glm::vec2           _position;
    glm::vec2           _direction;
    State::Enum         _state;
    float               _speed;
    Type::Enum          _type;
};

#endif
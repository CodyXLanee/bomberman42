/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:28:44 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 12:27:56 by egaborea         ###   ########.fr       */
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
    AGameEntity(void);
    AGameEntity(AGameEntity const &);
    AGameEntity   &operator=(AGameEntity const &rhs);
    AGameEntity(const glm::vec2 &, const glm::vec2 &, const State::Enum &, const float &, const Type::Enum &);
    glm::vec2           _position;
    glm::vec2           _direction;
    State::Enum         _state;
    float               _speed;
    Type::Enum          _type;
};

#endif
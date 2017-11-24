/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEntity.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 15:01:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/24 15:33:54 by egaborea         ###   ########.fr       */
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
    
    virtual glm::vec2	getPositon();
    virtual State::Enum	getState();
    virtual float		getSpeed();
    virtual Type::Enum	getType();

    void	setPositon(const glm::vec2 &);
    void	setState(const State::Enum &);
    void	setSpeed(const float &);
    void	setType(const Type::Enum &);
private:
    GameEntity();
    glm::vec2           _position;
    State::Enum         _state;
    float               _speed;
    Type::Enum          _type;
};

#endif
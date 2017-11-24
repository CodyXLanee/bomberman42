/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEntity.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 15:01:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/24 17:28:20 by lfourque         ###   ########.fr       */
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
    virtual State::Enum	getState() const;
    virtual float		getSpeed() const;
    virtual Type::Enum	getType() const;

    void	setPosition(const glm::vec2 &);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bonus.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 10:02:52 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/11 11:54:23 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_HPP
# define BONUS_HPP

# include "AGameEntity.hpp"
# include "SEventManager.hpp"

namespace BonusType {
    enum Enum {BOMB_UP, FLAME_UP, SPEED_UP};
};

class Bonus : public AGameEntity {
public:
    Bonus(const glm::vec2 & pos, BonusType::Enum type);
    ~Bonus();
    BonusType::Enum     getBonusType(void);

private:
    void    playerMoveCallback(void *);
    
    BonusType::Enum     _bonus_type;
};

#endif
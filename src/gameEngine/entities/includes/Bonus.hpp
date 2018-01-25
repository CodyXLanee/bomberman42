/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bonus.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 10:02:52 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 10:32:27 by egaborea         ###   ########.fr       */
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

    Bonus(void);
    Bonus(Bonus const &);
    Bonus   &operator=(Bonus const &rhs);

private:
    void    playerMoveCallback(void *);
    
    BonusType::Enum     _bonus_type;
};

#endif
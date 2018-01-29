/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombParticles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 14:51:42 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/29 10:18:52 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBPARTICLES_HPP
# define BOMBPARTICLES_HPP

# include "AParticleSystem.hpp"
# include <cmath>

# define PARTICLES_BY_BOMB 3

class BombParticles : public AParticleSystem {
    public:
    	BombParticles();
		BombParticles(bool active, glm::vec3 position);
		~BombParticles();

		BombParticles(BombParticles const & src);
		BombParticles& operator=(BombParticles const & rhs);
    
        void    addBombPlace(glm::vec2 pos);
        void    removeBombPlace(glm::vec2 pos);
        
    private:
        void    update();
        std::vector<std::pair<glm::vec3, std::vector<glm::vec3>>>  bombPlaces;
};

#endif
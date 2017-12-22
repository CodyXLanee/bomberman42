/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireParticles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 11:49:34 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 14:21:46 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREPARTICLES_HPP
# define FIREPARTICLES_HPP

# include "AParticleSystem.hpp"

class FireParticles : public AParticleSystem {
    public:
    	FireParticles();
		FireParticles(bool active, glm::vec3 position);
		~FireParticles();
    
        void    addFirePlace(glm::vec2 pos);
        
    private:
        void    update(Shader &shader);
        std::vector<std::pair<glm::vec3, std::vector<glm::vec3>>>  firePlaces;
};

#endif
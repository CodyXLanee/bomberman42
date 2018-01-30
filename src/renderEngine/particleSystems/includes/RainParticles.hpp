/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainParticles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 11:49:34 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/30 15:35:21 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAINPARTICLES_HPP
# define RAINPARTICLES_HPP

# include "AParticleSystem.hpp"

class RainParticles : public AParticleSystem {
    public:
    	RainParticles();
		RainParticles(bool active, glm::vec3 position);
		~RainParticles();

		RainParticles(RainParticles const & src);
		RainParticles& operator=(RainParticles const & rhs);
        
    private:
        void                    update();
        glm::vec3               oriPos;
        std::vector<glm::vec3>  positions;
};

#endif
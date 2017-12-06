/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:24 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 17:13:52 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEATHERSYSTEM_HPP
# define WEATHERSYSTEM_HPP

# include <glm/vec3.hpp>
# include "ParticleSystem.hpp"
# include "Model.hpp"
# include "Shader.hpp"
# include "Light.hpp"

class WeatherSystem {
	public:
		enum time { MIDDAY, MIDNIGHT, TWILIGHT };

		WeatherSystem();
		WeatherSystem(time t, bool cloudy, bool rainy);
		~WeatherSystem();
	
		void	setTime(time t);
		void	startCloud(void*);
		void	stopCloud(void*);
		void	startRain(void*);
		void	stopRain(void*);

		void	renderRain(Shader &shader) const;
		void	renderCloud(Shader &shader);
		
		glm::vec3	getLightingValues() const;
		Light		&getSun() const;
		
	private:

		void	init();
		void	initClouds();

		time			dayTime;
		bool cloudy;
		bool rainy;

		glm::vec3				lightingValues; //ambiant, diffuse, specular
		Light					*sun;
		Model					*cloudModel;
		ParticleSystem			*rain;
		std::vector<glm::vec3>	cloudsPosition;
		// std::vector<glm::vec3>	cloudsVelocity;
		// glm::vec3		wind;
};

#endif
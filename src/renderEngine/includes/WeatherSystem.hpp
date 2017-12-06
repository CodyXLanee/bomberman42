/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:24 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 15:09:47 by tpierron         ###   ########.fr       */
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
		void	startCloud();
		void	stopCloud();
		void	startRain();
		void	stopRain();

		void	renderRain(Shader &shader) const;
		void	renderCloud(Shader &shader) const;
		
		glm::vec3	getLightingValues() const;
		Light		&getSun() const;
		
	private:

		void	init();

		time			dayTime;
		bool cloudy;
		bool rainy;

		glm::vec3		lightingValues; //ambiant, diffuse, specular
		Light			*sun;
		Model			*cloudModel;
		ParticleSystem	*rain;
		glm::vec3		wind;
};

#endif
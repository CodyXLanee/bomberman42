/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:24 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/30 15:42:03 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEATHERSYSTEM_HPP
# define WEATHERSYSTEM_HPP

# include <glm/vec3.hpp>
# include "RainParticles.hpp"
# include "Model.hpp"
# include "Shader.hpp"
# include "Light.hpp"

class WeatherSystem {
	public:
		enum time { MIDDAY, TWILIGHT, MIDNIGHT };

		WeatherSystem();
		WeatherSystem(time t, bool cloudy, bool rainy);
		~WeatherSystem();

		WeatherSystem(WeatherSystem const & src);
		WeatherSystem&	operator=(WeatherSystem const & rhs);
	
		void	setTime(time t);
		void	startCloud(void*);
		void	stopCloud(void*);
		void	startRain(void*);
		void	stopRain(void*);

		void	renderRain(Shader &shader);
		void	renderCloud(Shader &shader);
		
		glm::vec3	const &getLightingValues() const;
		Light		&getSun() const;

		void		setSunTheme(void *p);
		
	private:

		void	init();
		void	initClouds();

		time			dayTime;
		bool			cloudy;
		bool			rainy;

		glm::vec3				lightingValues; //ambiant, diffuse, specular
		Light					*sun;
		Model					*cloudModel;
		RainParticles			rain;
		std::vector<glm::vec3>	cloudsPosition;
		// std::vector<glm::vec3>	cloudsVelocity;
		// glm::vec3		wind;
};

#endif
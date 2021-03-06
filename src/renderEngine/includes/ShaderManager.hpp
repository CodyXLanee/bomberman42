/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:17:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/14 10:35:11 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERANAGER_HPP
# define SHADERANAGER_HPP

# include "Shader.hpp"

class ShaderManager {
	public:
		ShaderManager();
		~ShaderManager();

		ShaderManager(ShaderManager const & src);
		ShaderManager&	operator=(ShaderManager const & rhs);

		Shader	&getMainShader() const;
		Shader	&getDirectionalShadowShader() const;
		Shader	&getPointShadowShader() const;
		Shader	&getFlamesShader() const;
		Shader	&getParticlesShader() const;

		void	setCamera(glm::mat4 const & camMat) const;
		void	setLightingValues(glm::vec3 const &values) const;
	private:

		std::string	directory;

		Shader		*mainShader;
		Shader		*flamesShader;
		Shader		*directionalShadowShader;
		Shader		*pointShadowShader;
		Shader		*particlesShader;
};

#endif
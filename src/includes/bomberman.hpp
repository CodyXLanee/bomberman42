/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomberman.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 15:58:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/28 16:20:43 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBERMAN_HPP
# define BOMBERMAN_HPP

# define WINDOW_WIDTH 1920.f
# define WINDOW_HEIGHT 1080.f
# define FOV 60.f
# define Z_NEAR 0.1f
# define Z_FAR 4000.f


namespace Action {
	enum Enum	{
		NONE,
		LEFT, RIGHT, UP, DOWN,
		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN,
		ESCAPE, DEBUG_MODE, RESET_CAMERA, MENU, OPTIONS
	};
}

#endif
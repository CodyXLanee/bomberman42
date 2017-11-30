/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomberman.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 15:58:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/30 10:49:19 by egaborea         ###   ########.fr       */
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
		LEFT, RIGHT, UP, DOWN, SPAWN_BOMB,
		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN,
		ESCAPE, DEBUG_MODE, FOLLOW_PLAYER, RESET_CAMERA, MENU, OPTIONS,
		BOMB_EXPLODES
	};
}

#endif
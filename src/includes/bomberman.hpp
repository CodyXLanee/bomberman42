/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomberman.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 15:58:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 10:15:20 by tpierron         ###   ########.fr       */
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
	enum Enum { NONE, LEFT, RIGHT, UP, DOWN, DEBUG, ESCAPE};
}

#endif
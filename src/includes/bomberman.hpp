/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomberman.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 15:58:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/05 17:41:07 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBERMAN_HPP
# define BOMBERMAN_HPP

# define FOV 60.f
# define Z_NEAR 0.1f
# define Z_FAR 4000.f


namespace Action {
	enum Enum	{
		NONE,
		LEFT, RIGHT, UP, DOWN, SPAWN_BOMB,
		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN,
		ESCAPE, DEBUG_MODE, FOLLOW_PLAYER, RESET_CAMERA, MENU, OPTIONS, KEY_BINDINGS,
		BOMB_EXPLODES, SCREEN_CHANGED
	};
}

namespace Screen {
	enum Resolution {
		RES_2560_1440 = 0,
		RES_1920_1080,
		RES_1024_768
	};
	enum Mode {
		WINDOWED = 0,
		FULLSCREEN
	};
	struct Format {
		Screen::Resolution	resolution;
		Screen::Mode		mode;
	};
}

namespace Menu {
	enum Enum {NONE, DEBUG, BASE, OPTIONS, KEY_BINDINGS, START};
}

namespace GameMode {
    enum Enum {BRAWL, CAMPAIGN};
};

namespace Save {
    enum Enum {SLOT1, SLOT2, SLOT3};
};

namespace VolumeType {
    enum Enum {MASTER, MUSIC, SOUND};
};

namespace WinCondition {
	enum Enum {NO_MORE_BLOCKS, NO_MORE_ENEMIES};
};

#endif
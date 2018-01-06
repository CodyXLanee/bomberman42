/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomberman.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 15:58:49 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/04 16:40:44 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBERMAN_HPP
# define BOMBERMAN_HPP

# define FOV 60.f
# define Z_NEAR 0.1f
# define Z_FAR 4000.f

# include <SDL.h>

namespace Screen {
	enum WindowMode {
		WINDOWED = 0,
		FULLSCREEN
	};
	struct Format {
		SDL_DisplayMode		displayMode;
		Screen::WindowMode	windowMode;
	};
}

namespace Menu {
	enum Enum {NONE, DEBUG, BASE, OPTIONS, KEY_BINDINGS, START, LEVEL_SELECTION, SELECT_SLOT, NEW_BRAWL, HOW_TO_PLAY};
}

namespace GameMode {
    enum Enum {BRAWL, CAMPAIGN};
};

namespace Difficulty {
	enum Enum {EASY, MEDIUM, HARD};
}

namespace PlayerColor {
	enum Enum {WHITE, BLACK, RED, YELLOW};
}

namespace Level {
	enum Enum {ONE, TWO, THREE, FOUR, FIVE, SIX}; // Levels should be renamed properly accordingly to their 'style' ?
}

namespace Save {
    enum Enum {SLOT1, SLOT2, SLOT3};
};

namespace UIAudio {
    enum Enum { HOVER, CLICK };
};

namespace WinCondition {
	enum Enum {NO_MORE_BLOCKS, NO_MORE_ENEMIES};
};

namespace Animation {
	enum Enum {START, WIN, GAME_OVER};
};

#endif
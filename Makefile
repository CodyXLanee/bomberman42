.PHONY: all clean fclean re install

NAME = bomberman

CC = clang++
CFLAGS = -Wall -Wextra -Werror -g -ggdb

SRC_PATH = ./src/
SRC_NAME =	\
			Sdl_gl_win.cpp \
			gameEngine/GameEngine.cpp \
			gameEngine/GameParams.cpp \
			gameEngine/AGameEntity.cpp \
			gameEngine/Loader.cpp \
			gameEngine/PlayerManager.cpp \
			gameEngine/WinManager.cpp \
			gameEngine/CollisionsManager.cpp \
			gameEngine/BombManager.cpp \
			gameEngine/EnemyManager.cpp \
			gameEngine/BonusManager.cpp \
			gameEngine/AI.cpp \
			gameEngine/Slot.cpp \
			gameEngine/entities/Bomb.cpp\
			gameEngine/entities/Bonus.cpp\
			gameEngine/entities/Enemy.cpp\
			gameEngine/entities/Flame.cpp\
			gameEngine/entities/DestructibleBloc.cpp\
			gameEngine/entities/IndestructibleBloc.cpp\
			gameEngine/entities/Player.cpp\
			gameEngine/entities/WinSpot.cpp\
			gameEngine/SoundManager.cpp\
			renderEngine/RenderEngine.cpp \
			renderEngine/NuklearGUI.cpp \
			renderEngine/model/Model.cpp \
			renderEngine/model/Mesh.cpp \
			renderEngine/model/mathTools.cpp \
			renderEngine/particleSystems/AParticleSystem.cpp \
			renderEngine/particleSystems/FireParticles.cpp \
			renderEngine/particleSystems/BombParticles.cpp \
			renderEngine/particleSystems/RainParticles.cpp \
			renderEngine/Shader.cpp \
			renderEngine/Camera.cpp \
			renderEngine/Light.cpp \
			renderEngine/WeatherSystem.cpp \
			renderEngine/ModelManager.cpp \
			renderEngine/ShaderManager.cpp \
			Map.cpp \
			main.cpp \
			SEventManager.cpp \
			SGameManager.cpp\

OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

INC_PATH = src/includes/ \
			src/gameEngine/includes \
			src/gameEngine/entities/includes\
			src/renderEngine/includes \
			src/renderEngine/model/includes \
			src/renderEngine/particleSystems/includes \
			libs

PACKAGES = sdl2 sdl2_mixer freetype2 glm rapidjson
ASSIMP_PATH =$(subst include/assimp,include/, $(shell ~/.brew/bin/pkg-config --cflags assimp))
ASSIMP_LIB =$(shell ~/.brew/bin/pkg-config --libs assimp)

PATHS = $(shell ~/.brew/bin/pkg-config --cflags $(PACKAGES)) \

LIBS = $(shell ~/.brew/bin/pkg-config --libs $(PACKAGES))

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
INC = $(subst $() $(), -I, $(INC_PATH))


all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src/gameEngine 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src/gameEngine/entities 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src/renderEngine 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src/renderEngine/model 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src/renderEngine/particleSystems 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/libs 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c $(INC) -Isrc $(PATHS) $(ASSIMP_PATH) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(LIBS) $(ASSIMP_LIB) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

install:
	./script.sh
	make
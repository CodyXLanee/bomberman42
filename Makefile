.PHONY: all clean fclean re install

NAME = bomberman

CC = clang++
CFLAGS = -Wall -Wextra -Werror

SRC_PATH = ./src/
SRC_NAME =	\
			Sdl_gl_win.cpp \
			main.cpp \

OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

INC_PATH = src/includes/

PACKAGES = sdl2 freetype2 glm

PATHS = $(shell pkg-config --cflags $(PACKAGES))
LIBS = $(shell pkg-config --libs $(PACKAGES))

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
INC = $(subst $() $(), -I, $(INC_PATH))


all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/libs 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c $(INC) -Isrc $(PATHS) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(LIBS) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

install:
	./script.sh
	make